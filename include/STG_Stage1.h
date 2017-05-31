#include "HfCloud.h"
#include <unordered_map>
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
    bool disposed;
    Sprite *sp_back;

    FPS_Counter fps;
public:
    std::vector<std::function<void(void)> > tasks;  //render tasks
    std::map<std::pair<int, int>, std::function<void(void)> >key_handlers;
    std::function<void(void)> logic_updater;

    Module *battle_module;
    virtual void start_scene(){
        Scene::start_scene();

        //---
        sp_back = new Sprite("sources/stage/back.png");
        main_module->manage(sp_back);

        battle_module = new Module(25, 25, 400, 430);

        fps.start();

        disposed = false;
    }
    virtual void end_scene(){
        if(disposed)return;
        Scene::end_scene();

        //----
        key_handlers.clear();
        tasks.clear();

        //
        auto dispose = [&](Sprite *s){
            s->bitmap->dispose();
            delete s->bitmap;
            s->dispose();
            delete s;
        };
        dispose(sp_back);

        delete battle_module;

        disposed = true;
    }
    virtual void update(){
        Scene::update();
        if (disposed)return;
        for(auto &task : tasks)task();
        tasks.clear();
        for(auto &handler : key_handlers){
            int key = handler.first.first;
            int state = handler.first.second;
            if(Input::key_is_state(key, state))
                handler.second();
            if(disposed)break;
        }
        if (disposed)return;

        battle_module->update();

        logic_updater();

        fps.addcnt();
    }
};
struct Player{
    bool disposed;
    int dir;  //moving dir 0 stand 2 up 8 down 4 left 6 right. 10-dir
    Bitmap *player_map;
    Bitmap *player_bmp;
    Sprite *sprite;
    SceneStage1 *scene;
    int x, y, speed; double SQRT2;

    static const int SPEED_FAST = 5;
    static const int SPEED_SLOW = 2;

    void init(SceneStage1 *s){
        scene = s;
        player_map = new Bitmap("sources/th14/player/pl00/pl00.png");
        player_bmp = new Bitmap(32, 48);
        sprite = new Sprite(player_bmp);
        sprite->opacity = 0;
        x = 400/2-32/2, y = 430-48; SQRT2 = sqrt(2);
        speed = SPEED_FAST;
        scene->tasks.push_back([&](){
            scene->battle_module->manage(sprite);
            sprite->setpos(x, y);
            sprite->opacity = 255;
        });
        disposed = false;

        dir = 2;
        update();
    }
    void destroy(){
        if(disposed)return;
        player_bmp->dispose();
        delete player_bmp;
        player_map->dispose();
        delete player_map;
        sprite->dispose();
        delete sprite;
        disposed = true;
    }

    void update(){
        if(disposed)return;
        #define calc(x, y) {x*32, y*48}
        //speed
        static int pos[10][2] = {
        calc(2, 0),   //0
        {0,0},
        calc(1, 0),   //2
        {0,0},
        calc(4, 1),    //4
        {0,0},
        calc(4, 2),    //6
        {0,0},
        calc(0, 2),    //8
        {0,0}
        };
        player_bmp->clear();
        player_bmp->blt(HfRect(0, 0, 32, 48), player_map, HfRect(pos[dir][0], pos[dir][1], 32, 48));
        if(speed == SPEED_SLOW){
            player_bmp->fill_rect(32/2-8/2, 48/2-8/2, 8, 8, RGBA(0xff, 0xff, 0xff, 0xff));
            player_bmp->fill_rect(32/2-6/2, 48/2-6/2, 6, 6, RGBA(0xff, 0, 0, 0xff));
        }
        sprite->update();
        dir = 2;
        #undef calc

        //speed
    }
    inline void pos_correct(){
        if(x < 0)x = 0;
        if(y < 0)y = 0;
        if(x > 400-32)x = 400-32;
        if(y > 430-48)y = 430-48;
        sprite->setpos(x, y);
    }
    void on_keyup(){
        dir = 2;
        y -= (Input::key_is_pressed(SDLK_LEFT) || Input::key_is_pressed(SDLK_RIGHT))?(int)(1.0*speed/SQRT2):speed;
        pos_correct();
    }
    void on_keydown(){
        dir = 8;
        y += (Input::key_is_pressed(SDLK_LEFT) || Input::key_is_pressed(SDLK_RIGHT))?(int)(1.0*speed/SQRT2):speed;
        pos_correct();
    }
    void on_keyleft(){
        dir = 4;
        x -= (Input::key_is_pressed(SDLK_UP) || Input::key_is_pressed(SDLK_DOWN))?(int)(1.0*speed/SQRT2):speed;
        pos_correct();
    }
    void on_keyright(){
        dir = 6;
        x += (Input::key_is_pressed(SDLK_UP) || Input::key_is_pressed(SDLK_DOWN))?(int)(1.0*speed/SQRT2):speed;
        pos_correct();
    }
    void on_keyshift(bool keyup = false){
        if(keyup && speed == SPEED_SLOW)speed = SPEED_FAST;
        else if(!keyup)speed = SPEED_SLOW;
    }
};
struct PlayerShoot{
    bool disposed;
    SceneStage1 *scene;

    Bitmap *shoot_map;
    Bitmap *shoot_bmp;

    Bitmap *shoots_map;
    Bitmap *shoots_bmp;
    Sprite *shoots_sprite;
    HfRect shoots_rect;

    bool showing;

    std::function<std::pair<int, int>(void)> get_player_pos;
    void init(SceneStage1 *s){
        scene = s;

        shoot_map = new Bitmap("sources/th14/player/pl00/pl00.png");
        shoot_bmp = new Bitmap(20, 16);
        shoot_bmp->blt_ex(HfRect(4, 0, 16, 16), shoot_map, HfRect(240, 48*3, 16, 16), 255, HfPoint(8, 8), 90, false, false);
        shoot_bmp->blt_ex(HfRect(2, 2, 4, 16), shoot_map, HfRect(128, 48*3, 16, 4), 255, HfPoint(2, 8), 0, false, true);

        shoots_map = new Bitmap(40, 16*27);
        for(int i = 0; i < 27; i++){
            int y = i*16;
            shoots_map->blt_ex(HfRect(0, y, 20, 16), shoot_bmp, HfRect(0, 0, 20, 16), 255, HfPoint(0, 0), 0, false, false);
            shoots_map->blt_ex(HfRect(20, y, 20, 16), shoot_bmp, HfRect(0, 0, 20, 16), 255, HfPoint(0, 0), 0, true, false);
        }
        shoots_bmp = new Bitmap(40, 16*27);
        shoots_bmp->clear();
        shoots_sprite = new Sprite(shoots_bmp);
        //shoots_bmp->blt(HfRect(0, 0, 40, 16*27), shoots_map, HfRect(0, 0, 40, 16*27));
        scene->tasks.push_back([&](){
            scene->battle_module->manage(shoots_sprite);
        });

        showing = false;
        disposed = false;
    }
    void destroy(){
        if(disposed)return;
        shoot_map->dispose();
        shoot_bmp->dispose();
        delete shoot_map;
        delete shoot_bmp;
        shoots_bmp->dispose();
        shoots_map->dispose();
        delete shoots_bmp;
        delete shoots_map;
        shoots_sprite->dispose();
        delete shoots_sprite;
        disposed = true;
    }
    void update(){
        if(disposed)return;
        shoots_bmp->clear();
        if(!showing)return;

        std::pair<int, int> pos = get_player_pos();
        int x = pos.first-(40-32)/2, y = pos.second-27*16;
        shoots_sprite->setpos(x, y);

        static const int opasn = 9; //opacity chaging
        static int opas[opasn] = { 255, 240, 220, 190, 170, 150, 140, 120, 100 };
        static int popas = 0;
        shoots_sprite->opacity = opas[(popas++)%opasn];
        shoots_bmp->blt_ex(HfRect(0, 0, 40, 16*27), shoots_map, HfRect(0, 0, 40, 16*27), 255, HfPoint(0, 0), 0, false, opasn<4);

    }
};
struct PlayerBomb{
    bool disposed;
    Bitmap *light_bmp;
    Bitmap *mop_bmp;
    Bitmap *bomb_bmp;
    Sprite *bomb_sprite;
    SceneStage1 *scene;
    bool showing;
    int show_time_left;
    int angle;
    int angle_sig;
    std::function<std::pair<int, int>(void)> get_player_pos;
    std::map<std::pair<int, int>, std::function<void(void)> >::iterator pshift;

    static const int BOMB_WIDTH = 220;
    static const int BOMB_HEIGHT = 180;
    static const int BOMB_TIME = 60*7;
    static const int BOMB_FIBER = FIBER_USER+1;
    static const int LIGHT_SIZE = 80;

    void init(SceneStage1 *s){
        scene = s;

        light_bmp = new Bitmap("sources/th14/player/pl00/pl00b.png");
        mop_bmp = new Bitmap("sources/th14/player/pl00/pl00f.png");

        bomb_bmp = new Bitmap(BOMB_WIDTH, BOMB_HEIGHT);
        bomb_bmp->clear();
        bomb_sprite = new Sprite(bomb_bmp);

        scene->tasks.push_back([&]() {
            scene->battle_module->manage(bomb_sprite);
            bomb_sprite->setz(-100);
        });

        pshift = scene->key_handlers.find(std::make_pair(SDLK_LSHIFT, Input::KEY_IS_PRESSED));  //找到按下shift的.

        showing = false;
        disposed = false;

    }
    void destroy() {
        if(disposed)return;
        light_bmp->dispose();
        delete light_bmp;
        mop_bmp->dispose();
        delete mop_bmp;
        bomb_bmp->dispose();
        delete bomb_bmp;
        bomb_sprite->dispose();
        delete bomb_sprite;
        if(Fiber::fiber().exist(BOMB_FIBER) && !Fiber::fiber().isDead(BOMB_FIBER))
            Fiber::fiber().kill(BOMB_FIBER);

        disposed = true;
    }
    void show_fiber_proc(){
        while(show_time_left--){
            std::pair<int, int> player_pos = get_player_pos();
            int player_x = player_pos.first, player_y = player_pos.second;
            bomb_sprite->setpos(player_x+16-BOMB_WIDTH/2, player_y-BOMB_HEIGHT/2);

            bomb_bmp->clear();
            bomb_bmp->blt(HfRect((bomb_sprite->width()-LIGHT_SIZE)/2, (player_y-bomb_sprite->y())/2+LIGHT_SIZE/3, LIGHT_SIZE, LIGHT_SIZE),
                                 light_bmp, HfRect(0, 0, light_bmp->width(), light_bmp->height()));

            bomb_bmp->blt_ex(HfRect(player_x-bomb_sprite->x()+LIGHT_SIZE/2+15, player_y-bomb_sprite->y()-90/2, 90, 90),
                                 mop_bmp, HfRect(0, 92, mop_bmp->width(), mop_bmp->height()-92),
                                 255,
                                 HfPoint(-LIGHT_SIZE/2, 90-30),
                                 angle, false, false);

            if(angle <= 0)angle_sig = 1;
            else if(angle >= 180)angle_sig = -1;

            if(angle <= 30 || angle >= 150)angle += angle_sig*3;
            else angle += angle_sig*6;

            pshift->second();  //强制低速模式
            Fiber::fiber().yield();
        }
    }
    void show(){
        if(disposed || showing)return;

        showing = true;
        show_time_left = BOMB_TIME;
        angle = 0, angle_sig = 1;
        Fiber::fiber()[BOMB_FIBER] = [&](){show_fiber_proc();};

    }
    void update(){
        if(disposed)return;

        if(Fiber::fiber().exist(BOMB_FIBER) && Fiber::fiber().isHungUp(BOMB_FIBER)){
            if(show_time_left) //进入Fiber继续更新bomb动画
                Fiber::fiber().resume(BOMB_FIBER);
            else{       //动画显示完毕
                bomb_bmp->clear();
                showing = false;
                Fiber::fiber().kill(BOMB_FIBER); //顺便干掉Fiber
            }
        }

    }
};
class STAGE1{
    SceneStage1 *scene;
    Player player;
    PlayerShoot pshoot;
    PlayerBomb pbomb;

    void leave(){
        player.destroy();
        pshoot.destroy();
        pbomb.destroy();
        scene->update_wait(20);
    }
public:
    void start(){
        SceneManager::call(scene = new SceneStage1);
        scene->logic_updater = [&,this](){update();};

        scene->key_handlers[std::make_pair(SDLK_ESCAPE, Input::KEY_IS_TRIGGLED)] = [&,this](){leave(); SceneManager::jumpback();};

        player.init(scene);
        scene->key_handlers[std::make_pair(SDLK_UP, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyup();};
        scene->key_handlers[std::make_pair(SDLK_DOWN, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keydown();};
        scene->key_handlers[std::make_pair(SDLK_LEFT, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyleft();};
        scene->key_handlers[std::make_pair(SDLK_RIGHT, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyright();};
        scene->key_handlers[std::make_pair(SDLK_LSHIFT, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyshift(false);};
        scene->key_handlers[std::make_pair(SDLK_LSHIFT, Input::KEY_IS_UNPRESSED)] = [&,this](){player.on_keyshift(true);};

        pshoot.init(scene);
        pshoot.get_player_pos = [&](){return std::make_pair(player.x, player.y);};

        scene->key_handlers[std::make_pair(SDLK_z,Input::KEY_IS_PRESSED)] = [&, this](){pressed_z();};
        scene->key_handlers[std::make_pair(SDLK_z, Input::KEY_IS_UNPRESSED)] = [&, this](){unpressed_z(); };

        pbomb.init(scene);
        pbomb.get_player_pos = [&](){return std::make_pair(player.x, player.y);};
        scene->key_handlers[std::make_pair(SDLK_x, Input::KEY_IS_TRIGGLED)] = [&, this](){pbomb.show();};

        SceneManager::scene->update_wait(20);
    }
    void update(){
        player.update();
        pshoot.update();
        pbomb.update();
    }
    void pressed_z(){
        pshoot.showing = true;
    }
    void unpressed_z(){
        pshoot.showing = false;
    }
}stage1;
