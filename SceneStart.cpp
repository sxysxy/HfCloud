#include "HfCloud.h"
#include "SceneStart.h"
#include "SceneMVC.h"
#include "STG_Stage1.h"
using namespace HfCloud;

class SceneStartLogic{
    bool disposed;
    Sprite *title_back;
    Sprite *title_circle;
    Sprite *title_cursor;
    SceneMVC *scene;
    int cursor_pos;
public:
    void init(SceneMVC *s){
        scene = s;
        Graphics::set_title(HfText("东方神圣魔王 -- Lunatic of Holy Kingdom ~v0.01a"));
        title_back = new Sprite("sources/title/back.png");
        title_circle = new Sprite("sources/title/circle.png");
        title_circle->show_rect.x = 450, title_circle->show_rect.y = 350;
        title_circle->rcenter.x = title_circle->width()/2;
        title_circle->rcenter.y = title_circle->height()/2;
        title_cursor = new Sprite("sources/title/cursor.png");
        title_cursor->opacity = 150;
        cursor_pos = 0;

        scene->register_key(make_keyentry(SDLK_UP, Input::KEY_IS_TRIGGLED), [&]{cursor_pos^=1;});
        scene->register_key(make_keyentry(SDLK_DOWN, Input::KEY_IS_TRIGGLED), [&]{cursor_pos^=1;});
        scene->register_key(make_keyentry(SDLK_z, Input::KEY_IS_TRIGGLED), [&]{triggled_z();});

        disposed = false;
    }
    void destroy(){
        if(!disposed){
            title_back->bitmap->dispose();
            delete title_back->bitmap;
            delete title_back;
            title_circle->bitmap->dispose();
            delete title_circle->bitmap;
            delete title_circle;
            title_cursor->bitmap->dispose();
            delete title_cursor->bitmap;
            delete title_cursor;
            disposed = true;
        }
    }
    void update(){
        title_circle->angle += 3;
        title_cursor->setpos(350, 170+80*cursor_pos);

        scene->register_task([&](){
            title_back->update();
            title_circle->update();
            title_cursor->update();
        });
    }
    void triggled_z(){
        switch(cursor_pos){
        case 0:
            stage1.start();
        break;
        case 1:
            SceneManager::exit();
        }
    }
};
static SceneStartLogic starter;
void SceneStart::start_scene(){
    SceneMVC::start_scene();
    starter.init(this);
    logic_updater = [&]{starter.update();};
}
void SceneStart::end_scene(){
    SceneMVC::end_scene();
    starter.destroy();
}
