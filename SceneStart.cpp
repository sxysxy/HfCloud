#include "HfCloud.h"
#include "SceneStart.h"
#include "SDL_timer.h"
using namespace HfCloud;
//This is always the first scene
struct{

    int cnt;
    long long last;
    long long now;
    long long freq;
    char buf[233];
    SDL_Texture *tex;
}data;
void SceneStart::start_scene(){


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
