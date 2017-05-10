#include "SceneManager.h"
#include "SceneStart.h"
using namespace HfCloud;

//defines SceneManage's static members here. DO NOT DEFINE IT IN HEADER FILES.
/*
extern Scene *SceneManager::scene;
extern std::vector<Scene *> SceneManager::scenes;
*/

extern std::vector<Scene *> del_scenes;  //when a scene is deleted, it will be pushed in it.
                        //You must want to ask me why don't delete it directly?
                        //I do not know why it will causs segment fault if I delete it directly.

void SceneManager::clear(){
    auto del = [](std::vector<Scene *> &ss){ //__del_scenes, delete all scenes in a std::vector
        for(Scene *s : ss)if(s)delete s;
    };
    del(SceneManager::scenes);
    del(del_scenes);
    del_scenes.clear();
    scenes.clear();
    scenes.push_back(NULL);
}
void SceneManager::jumpto(Scene *scene){
    SceneManager::clear();
    if(SceneManager::scene)SceneManager::scene->end_scene();
    SceneManager::scene = scene;
}
void SceneManager::jumpback(){
    if(SceneManager::scene)SceneManager::scene->end_scene();
    del_scenes.push_back(SceneManager::scene);
    SceneManager::scene = scenes.back();
    scenes.pop_back();
}
void SceneManager::call(Scene *scene){
    if(SceneManager::scene)SceneManager::scene->end_scene();
    scenes.push_back(SceneManager::scene);
    SceneManager::scene = scene;
}
void SceneManager::run(Scene *scene){
    SceneManager::jumpto(scene);
    while(SceneManager::scene)SceneManager::scene->main_proc();
    SceneManager::clear();
}
void SceneManager::exit(){
    SceneManager::scene = NULL;
}
