#include "HfCloud.h"
#include "SceneStart.h"
#include "SDL_timer.h"
#include "Fiber.h"

using namespace HfCloud;
//This is always the first scene
const int waitd = 30;  //绘制一个正方形的时间间隔
const int rectn = 40;  //绘制40个
struct{
    Bitmap *bmp[rectn];
    Sprite *sprite[rectn];
    int cnt;
    long long last;
    long long now;
    long long freq;
    char buf[233];

}data;
extern Fiber fiber;
void SceneStart::start_scene(){
    for(int i = 0; i < 40; i++){
        data.bmp[i] = new Bitmap(50, 50);
        data.bmp[i]->fill_rect(0, 0, 50, 50, RGBA(0xff, 0, 0xff, 0xff));
    }
    int p = 0;
    for(int i = 0; i < 10; i += 2)for(int j = 0; j < 8; j ++){
        int f = j&1;
        data.sprite[p] = new Sprite(data.bmp[p]);
        data.sprite[p]->setpos((i+f)*50, j*50);
        p++;
    }
    data.freq = SDL_GetPerformanceFrequency();
    data.last = SDL_GetPerformanceCounter();
    fiber[1] = [&](){
        for(int i = 0; i < 40; i++){
            main_module->manage(data.sprite[i]);
            for(int i = 0; i < 30; i++)fiber.yield();
        }
    };
}
void SceneStart::end_scene(){
    for(int i = 0; i < 40; i++){
        if(data.bmp[i])delete data.bmp[i];
        if(data.sprite[i])delete data.sprite[i];
    }
    memset(data.bmp, 0, sizeof(data.bmp));
    memset(data.sprite, 0, sizeof(data.sprite));
}
void SceneStart::update(){
    Scene::update();
    if(fiber.status(1) == FiberStatus::HungUp)
        fiber.resume(1);
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
