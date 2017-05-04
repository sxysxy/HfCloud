#include "HfCloud.h"
#include "SceneStart.h"
using namespace HfCloud;
//This is always the first scene
struct{
    Bitmap *bmp;
    Sprite *sprite;
}data;

void SceneStart::start_scene(){
    data.bmp = new Bitmap(Graphics::width, Graphics::height);
    data.sprite = new Sprite(data.bmp);
}
void SceneStart::end_scene(){
    if(data.bmp){
        data.bmp->dispose();
        data.bmp = NULL;
    }
}
void SceneStart::update(){
    Scene::update();

    data.bmp->clear();
    Hf_Rect rt = (Hf_Rect){0, 0, 100, 100};
    data.bmp->fill_rect(rt, RGBA(255, 0, 0, 255));
    rt = (Hf_Rect){100, 100, 100, 100};
    data.bmp->fill_rect(rt, RGBA(0, 255, 0, 255));
    rt = (Hf_Rect){200, 200, 100, 100};
    data.bmp->fill_rect(rt, RGBA(0, 0, 255, 255));

    data.sprite->sync_texture();
    data.sprite->update();

    if(Input::key_is_triggled(SDLK_ESCAPE))SceneManager::exit(); //press esc to quit
}
