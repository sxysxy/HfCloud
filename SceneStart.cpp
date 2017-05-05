#include "HfCloud.h"
#include "SceneStart.h"
#include "SDL_timer.h"
using namespace HfCloud;
//This is always the first scene
struct{
    Bitmap *bmp;
    Sprite *sprite;
    int cnt;
    long long last;
    long long now;
    long long freq;
    char buf[233];
    SDL_Texture *tex;
}data;
void SceneStart::start_scene(){
    data.bmp = new Bitmap(100, 100);
    data.bmp->fill_rect(0, 0, 100, 100, RGBA(255, 255, 0, 255));
    data.sprite = new Sprite(data.bmp);
    data.sprite->setpos(100, 300);
    main_module->manage(data.sprite);


    data.freq = SDL_GetPerformanceFrequency();
    data.last = SDL_GetPerformanceCounter();
}
void SceneStart::end_scene(){
    if(data.bmp){
        data.bmp->dispose();
        data.bmp = NULL;
    }
    if(data.sprite){
        data.sprite->dispose();
        data.sprite = NULL;
    }
}
void SceneStart::update(){
    Scene::update();
    if(Input::key_is_triggled(SDLK_ESCAPE))SceneManager::exit(); //press esc to quit
    if(++data.cnt == 60){
        data.cnt = 0;
        data.now = SDL_GetPerformanceCounter();
        double c = 1.0*(data.now-data.last)/data.freq;
        sprintf(data.buf, "HfCloud  -FPS : %.2lf", 60/c);
        Graphics::set_title(data.buf);
        data.last = SDL_GetPerformanceCounter();
    }
}
