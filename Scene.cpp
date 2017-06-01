#include "Scene.h"
#include "SceneManager.h"
#include "Graphics.h"
#include "Input.h"
#include "Module.h"
#include "Fiber.h"
using namespace HfCloud;

Scene::Scene(){
    main_module = nullptr;
    fade_time = 30;
 //   fading = false;
}
Scene::~Scene(){
    delete main_module;
}

void Scene::start_scene(){
    main_module = new Module(0, 0, Graphics::width, Graphics::height);
    HFASSERT(main_module != nullptr, "Create main_module failed")
   // fading = true;
}
void Scene::end_scene(){
    delete main_module;
    main_module = nullptr;
}

void Scene::update_event(){
    SDL_Event eve;
    while(SDL_PollEvent(&eve)){
        if(eve.type == SDL_QUIT)SceneManager::exit(); //exit event, for example click 'x'
    }
}
void Scene::update(){
    update_event();
    if(main_module)main_module->update();
}
bool Scene::running(){
    return this == SceneManager::scene;
}


void Scene::main_proc(){
    if(!SceneManager::scene)return;
    fiber = &Fiber::fiber();
    (*fiber)[1] = [&]{
        while(true){
            Graphics::clear();              //clear the Graphics in order to redraw
            update();
            yield_for_wait();
        }
    };
#ifdef __LINUX__
    delay_short();
#endif
    start_scene();
    while(this == SceneManager::scene){
        Input::update();
        fiber->resume(1);
        Graphics::update();
    }
    end_scene();
    fiber->destroy(1);
#ifdef __LINUX__
    delay_short();
#endif
}
void Scene::update_for_wait(){
    Input::update();
    update_event();
    Graphics::update();
}
void Scene::update_wait(int d){
    HFASSERT(d >= 0, "Wait time should >= 0")
    while(d--)update_for_wait();
}
void Scene::update_wait(int d, const std::function<void(int)> &updater){
    HFASSERT(d >= 0, "Wait time should >= 0")
    for(int i = 0; i < d; i++){
        updater(i);
        update_for_wait();
    }
}
void Scene::yield_for_wait(){
    fiber->yield();  //after yield it runs Graphics::update() and wait for 1 frame.
}
void Scene::wait(int d){
    HFASSERT(d >= 0, "Wait time should >= 0")
    while(d--)yield_for_wait();
}
void Scene::wait(int d, const std::function<void(int)> &updater){
    HFASSERT(d >= 0, "Wait time should >= 0")
    for(int i = 0; i < d; i++){
        updater(i);
        yield_for_wait();
    }
}
void Scene::delay(int dms){
    HFASSERT(dms >= 0, "delay time should >= 0")
    SDL_Delay(dms);
}
void Scene::delay_short(){
    delay(100);
}
