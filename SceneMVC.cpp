#include "SceneMVC.h"
#include "Input.h"
using namespace HfCloud;

void SceneMVC::register_key(const KeyEntry &entry, const std::function<void(void)> &handler){
    key_handlers[entry.hashcode] = handler;
}
void SceneMVC::register_task(const std::function<void(void)> &task){
    tasks.push_back(task);
}
void SceneMVC::update(){
    Scene::update();

    for(auto &f:tasks)f();
    tasks.clear();

    for(auto it = key_handlers.begin(); it != key_handlers.end(); ++it){
        KeyEntry entry(it->first);
        if(Input::key_is_state(entry.info.key, entry.info.state))
            it->second();
        if(!running())break;
    }
    if(!running())return;

    logic_updater();
}
void SceneMVC::start_scene() {
    logic_updater = (std::function<void(void)>)HfDummy;
}
void SceneMVC::end_scene(){
    tasks.clear();
    key_handlers.clear();
    logic_updater = (std::function<void(void)>)HfDummy;
}
