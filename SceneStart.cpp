#include "HfCloud.h"
#include "SceneStart.h"
#include "SDL_timer.h"
#include "Fiber.h"
#include "Animation.h"
#include "AnimationFrames.h"
using namespace HfCloud;
struct{
    int cnt;
    long long last;
    long long now;
    long long freq;
    char buf[233];

    AnimationFrames *ani;
    Bitmap *bmp;
}data;
void SceneStart::start_scene(){
    Graphics::resize(192, 192);         //resize
    data.freq = SDL_GetPerformanceFrequency();
    data.last = SDL_GetPerformanceCounter();

    data.bmp = new Bitmap("sources/animation.png");
    data.ani = new AnimationFrames(100);  //make a animation
    for(int i = 0; i < 100; i++){
        if(i%4){
            data.ani->ani_frames[i] = data.ani->ani_frames[i-1];
            continue;
        }
        Sprite *sp = new Sprite(data.bmp);
        sp->show_rect.x = 0, sp->show_rect.y = 0, sp->show_rect.w = 192, sp->show_rect.h = 192;
        int p = i/4;
        sp->clip_rect.x = (p%5)*192, sp->clip_rect.y = (p/5)*192;
        sp->clip_rect.w = 192, sp->clip_rect.h = 192;
        data.ani->ani_frames[i] = new AnimationFrame(sp);
    }
    auto hooked = data.ani->update_frame;  //hook the updater
    data.ani->update_frame = [=](Animation *ani){
        hooked(ani);
        if(ani->current_frame == ani->total_frames-1)
            ani->current_frame = 0;
    };
}
void SceneStart::end_scene(){
    data.bmp->dispose();
    delete data.bmp;
    for(int i = 0; i < 100; i += 4){
        data.ani->ani_frames[i]->sprite->dispose();
        delete data.ani->ani_frames[i]->sprite;
        delete data.ani->ani_frames[i];
    }
}
void SceneStart::update(){
    Scene::update();
    data.ani->update();

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
