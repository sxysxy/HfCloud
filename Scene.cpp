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

void Scene::main_proc(){
    if(!SceneManager::scene)return;
    fiber = &Fiber::fiber();

    (*fiber)[1] = [&](){
        while(true){
            Graphics::clear();              //clear the Graphics in order to redraw
            update();
            yield_for_wait();
        }
    };
    start_scene();
    while(this == SceneManager::scene){
        Input::update();                    //clear the input buffer
        fiber->resume(1);
        Graphics::update();
    }
    end_scene();
    fiber->kill(1);
}
void Scene::update_for_wait(){
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
    fiber->yield();
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
