#include "HfCloud.h"
using namespace HfCloud;

struct FPS_Counter{
    long long last;
    long long now;
    long long feq;
    const char *title;
    char buf[200];
    int count;
    void start(){
        feq = SDL_GetPerformanceFrequency();
        last = SDL_GetPerformanceCounter();
        count = 0;
    }
    void addcnt(){
        if(++count == 60){
            now = SDL_GetPerformanceCounter();
            double sec = 1.0*(now-last)/feq;
            printf("FPS : %.2lf\n", 60.0/sec);
            last = SDL_GetPerformanceCounter();
            count = 0;
        }
    }
};

class SceneStage1 : public Scene{
    Sprite *sp_back;
    Sprite *t;
    Module *battle_module;

    FPS_Counter fps;
public:
    std::vector<std::function<void(void)> > tasks;
    std::function<void(void)> key_handlers;
    virtual void start_scene(){
        sp_back = new Sprite("sources/stage/back.png");
        main_module->manage(sp_back);

        battle_module = new Module(25, 25, 400, 430);
        t = new Sprite("construction.png");
        t->resize(400, 430);
        t->scale_with_bitmap();
        battle_module->manage(t);

        fps.start();
    }
    virtual void end_scene(){

    }
    virtual void update(){
        Scene::update();

        for(auto &task : tasks)task();
        tasks.clear();

        battle_module->update();

        if(Input::key_is_triggled(SDLK_UP))t->setpos(t->x(), t->y()-1);
        if(Input::key_is_triggled(SDLK_DOWN))t->setpos(t->x(), t->y()+1);
        if(Input::key_is_triggled(SDLK_LEFT))t->setpos(t->x()-1, t->y());
        if(Input::key_is_triggled(SDLK_RIGHT))t->setpos(t->x()+1, t->y());

        fps.addcnt();
    }
};
struct MovingObjcet{
    int x, y;

};

class STAGE1{

    void leave(){

    }
public:
    void start(){
        SceneManager::jumpto(new SceneStage1);
    }
}stage1;
