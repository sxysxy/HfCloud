#include "HfCloud.h"
#include "SceneStart.h"
#include "SDL_timer.h"
#include "Fiber.h"
using namespace HfCloud;
struct{
    Sprite *sprite;
    int cnt;
    long long last;
    long long now;
    long long freq;
    char buf[233];
}data;
void SceneStart::start_scene(){
    data.sprite = new Sprite("construction.png");
    data.sprite->resize(Graphics::width, Graphics::height);
    data.sprite->setz(100);
    data.sprite->scale_with_bitmap();
    main_module->manage(data.sprite);
    data.freq = SDL_GetPerformanceFrequency();
    data.last = SDL_GetPerformanceCounter();

    Fiber::fiber()[1] = [&](){
        for(int i = 0; i < 100; i++){
            int x = data.sprite->x(), y = data.sprite->y();
            data.sprite->setpos(x+1, y+1);
            Fiber::fiber().yield();
        }
        data.sprite->setpos(-100, 100);
        for(int i = 0; i < 100; i++){
            int x = data.sprite->x(), y = data.sprite->y();
            data.sprite->setpos(x+1, y-1);
            Fiber::fiber().yield();
        }
    };

}
void SceneStart::end_scene(){}
void SceneStart::update(){
    Scene::update();
    try{
        if(Fiber::fiber().isHungUp(1))Fiber::fiber().resume(1);
    }catch(...){}

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
