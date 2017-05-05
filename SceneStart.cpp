#include "HfCloud.h"
#include "SceneStart.h"
#include "SDL_timer.h"
using namespace HfCloud;
//This is always the first scene
struct{
    Bitmap *bmp1, *bmp2;
    Sprite *sp1, *sp2;
    int cnt;
    long long last;
    long long now;
    long long freq;
    char buf[233];
    SDL_Texture *tex;
}data;
void SceneStart::start_scene(){
    data.bmp1 = new Bitmap(100, 100);
    data.bmp1->fill_rect(0, 0, 50, 50, RGBA(255, 255, 0, 255));
    data.bmp1->fill_rect(50, 50, 50, 50, RGBA(0, 255, 0, 255));
    data.sp1 = new Sprite(data.bmp1);
    data.sp1->setpos(100, 300);
    main_module->manage(data.sp1);
    data.bmp2 = new Bitmap(100, 100);
    data.bmp2->fill_rect(0, 0, 100, 100, RGBA(0, 255, 255, 255));
    data.sp2 = new Sprite(data.bmp2);
    data.sp2->setpos(300, 100);
    main_module->manage(data.sp2);

    data.freq = SDL_GetPerformanceFrequency();
    data.last = SDL_GetPerformanceCounter();
}
void SceneStart::end_scene(){

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
