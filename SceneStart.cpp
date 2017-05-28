#include "HfCloud.h"
#include "SceneStart.h"
#include "SDL_timer.h"
#include "Fiber.h"
#include "Animation.h"
#include "AnimationFrames.h"

#include "STG_Stage1.h"
using namespace HfCloud;
struct{
    bool disposed;
    Sprite *title_back;
    Sprite *title_circle;
    Sprite *title_cursor;
    int cursor_pos;

}t_data; //title_data;
void SceneStart::start_scene(){
    Graphics::set_title(u8"东方神圣魔王 -- Lunatic of Holy Kingdom ~v0.01a");
    t_data.title_back = new Sprite("sources/title/back.png");
    t_data.title_circle = new Sprite("sources/title/circle.png");
    t_data.title_circle->show_rect.x = 450, t_data.title_circle->show_rect.y = 350;
    t_data.title_circle->rcenter.x = t_data.title_circle->width()/2;
    t_data.title_circle->rcenter.y = t_data.title_circle->height()/2;
    t_data.title_cursor = new Sprite("sources/title/cursor.png");
    t_data.title_cursor->opacity = 150;
    t_data.cursor_pos = 0;

    t_data.disposed = false;
}
void SceneStart::end_scene(){
    if(!t_data.disposed){
        t_data.title_back->bitmap->dispose();
        delete t_data.title_back->bitmap;
        delete t_data.title_back;
        t_data.title_circle->bitmap->dispose();
        delete t_data.title_circle->bitmap;
        delete t_data.title_circle;
        t_data.title_cursor->bitmap->dispose();
        t_data.disposed = true;
    }
}
void SceneStart::update(){
    Scene::update();

    //update pictures
    t_data.title_back->update();
    t_data.title_circle->update();
    t_data.title_circle->angle++;

    //update curosr
    if(Input::key_is_triggled(SDLK_UP) || Input::key_is_triggled(SDLK_DOWN))
        t_data.cursor_pos ^= 1;
    t_data.title_cursor->show_rect.x = 350;
    t_data.title_cursor->show_rect.y = 170+t_data.cursor_pos*80;
    t_data.title_cursor->update();

    if(Input::key_is_triggled(SDLK_RETURN) || Input::key_is_triggled(SDLK_z)){ //return / z
        switch(t_data.cursor_pos){
        case 0:
            stage1.start();   //new game
        break;
        case 1:
            SceneManager::exit();  //exit
        break;
        }
    }
}
