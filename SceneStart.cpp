#include "HfCloud.h"
#include "SceneStart.h"
//This is always the first scene

struct{
    Sprite *bak, *frot;
}data;
void SceneStart::start_scene(){
    data.bak = new Sprite("t2.png");  //默认 x = y = z = 0
    data.frot = new Sprite("t1.png", 100, 100, 30); // (x, y, z), z = 30，不明的绿色物体在前
   // data.frot = new Sprite("t1.png", 100, 100, -20); // (x, y, z), z = -20, 看不到绿色物体

    main_module->manage(data.bak);
    main_module->manage(data.frot);
}
void SceneStart::end_scene(){

}
void SceneStart::update(){
    Scene::update();
    if(Input::key_is_triggled(SDLK_ESCAPE))SceneManager::exit();
}
