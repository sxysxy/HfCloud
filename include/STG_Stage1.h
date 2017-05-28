#include "HfCloud.h"
using namespace HfCloud;

class SceneStage1 : public Scene{
    Sprite *sp_back;

    Module *battle_module;
public:
    std::vector<std::function<void(void)> > tasks;
    std::function<void(void)> key_handlers;
    virtual void start_scene(){
        sp_back = new Sprite("sources/stage/back.png");
        main_module->manage(sp_back);

        battle_module = new Module(25, 25, 400, 430);
        Sprite *t = new Sprite(new Bitmap(400, 430));
        t->bitmap->fill_rect(0, 0, 400, 430, RGBA(0xff, 0, 0, 0xff));
        battle_module->manage(t);
    }
    virtual void end_scene(){

    }
    virtual void update(){
        Scene::update();

        for(auto &task : tasks)task();
        tasks.clear();

        battle_module->update();
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
