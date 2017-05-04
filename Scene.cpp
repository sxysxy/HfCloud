#include "Scene.h"
#include "SceneManager.h"
#include "Graphics.h"
#include "Input.h"
#include "Module.h"
using namespace HfCloud;

Scene::Scene(){
    main_module = new Module(0, 0, Graphics::width, Graphics::height);
}
Scene::~Scene(){
    delete main_module;
}

void Scene::start_scene(){
}
void Scene::end_scene(){
}

void Scene::update_event(bool update_keys = true){
    SDL_Event eve;
    while(SDL_PollEvent(&eve)){
        if(eve.type == SDL_QUIT)SceneManager::exit(); //exit event
                //input events
        if(!update_keys)continue;
        if(eve.type == SDL_KEYDOWN || eve.type == SDL_KEYUP){ //...
            Input::update(eve.key);
        //}else if(eve.type == SDL_MOUSEMOTION){
         //   Input::update(eve.motion);
        }else if(eve.type == SDL_MOUSEBUTTONDOWN || eve.type == SDL_MOUSEBUTTONUP){
            Input::update(eve.button);
        }
    }
}
void Scene::update(){
    update_event();
    main_module->update();
}

void Scene::main_proc(){
    if(!this)return;
    start_scene();
    while(this == SceneManager::scene){
        Graphics::clear();              //clear the Graphics in order to redraw
        Input::clear();                 //clear the input buffer
        update();                       //update
        Graphics::update();             //render the Graphics and update frame count.
    }
    end_scene();
}

void Scene::update_for_wait(){
    Graphics::update();
    update_event(false);  // process events in order not to die...
}
void Scene::wait(int d){
    while(d--)update_for_wait();
}
