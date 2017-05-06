#include "HfCloud.h"
#include "SceneStart.h"
#include "SDL_timer.h"
#include "process.h"
#include "windows.h"
using namespace HfCloud;
//This is always the first scene
const int waitd = 30;  //绘制一个正方形的时间间隔
const int rectn = 40;  //绘制40个
struct{
    Bitmap *bmp[rectn];
    Sprite *sprite[rectn];
    PVOID root_fiber;
    PVOID work_fiber;
    int cnt;
    long long last;
    long long now;
    long long freq;
    char buf[233];
    SDL_Texture *tex;
}data;
void WINAPI fiber_main(void *arg);
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
        //main_module->manage(data.sprite[p]);
        p++;
    }
    data.root_fiber = ConvertThreadToFiberEx(NULL, FIBER_FLAG_FLOAT_SWITCH);

    data.work_fiber = CreateFiberEx(0, 0, FIBER_FLAG_FLOAT_SWITCH, fiber_main, NULL);

    data.freq = SDL_GetPerformanceFrequency();
    data.last = SDL_GetPerformanceCounter();
}
void SceneStart::end_scene(){
    for(int i = 0; i < 40; i++){
        if(data.bmp[i])delete data.bmp[i];
        if(data.sprite[i])delete data.sprite[i];
    }
    memset(data.bmp, 0, sizeof(data.bmp));
    memset(data.sprite, 0, sizeof(data.sprite));
}
void WINAPI fiber_main(void *arg){
    for(int i = 0; i < 40; i++){
        SceneManager::scene->main_module->manage(data.sprite[i]);
        for(int i = 0; i < 30; i++)SwitchToFiber(data.root_fiber);
    }
    data.work_fiber = NULL;
    SwitchToFiber(data.root_fiber);
}
void update_fiber(){
    if(data.work_fiber)SwitchToFiber(data.work_fiber);
}
void SceneStart::update(){
    Scene::update();
    update_fiber();
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
