#include "HfCloud.h"
#include "Font.h"
#include <unordered_map>
using namespace HfCloud;

struct FPS_Counter{  //FPS 记录器，由addcnt函数负责更新，达到建议帧率(ADVISED_FPS)的时候会绘制一次帧率的数据。
    bool disposed;
    long long last;
    long long now;
    long long feq;
    char buf[100];
    int count;
    Font font;
    Sprite *fps_sprite;
    static const int ADVISED_FPS = 60;
    void start(){                   //启动
        feq = SDL_GetPerformanceFrequency();
        last = SDL_GetPerformanceCounter();
        count = 0;
        font.reload(u8"SIMYOU.TTF", 16);
        fps_sprite = new Sprite(new Bitmap(1, 1));      //这个1,1的位图是没用的。
        fps_sprite->setpos(510, 430);               //FPS显示的位置
        disposed = false;
    }
    void addcnt(){
        if(++count == ADVISED_FPS){   //达到次数
            fps_sprite->bitmap->dispose();
            delete fps_sprite->bitmap;

            now = SDL_GetPerformanceCounter();
            double sec = 1.0*(now - last) / feq;            //计算与上次达到的时间差
            sprintf(buf, "FPS : %.2lf", ADVISED_FPS / sec); //计算出帧率

            Bitmap *bmp = font.render_solid_text(buf, RGBA(0, 0xff, 0, 0xff));      //绘制字体
            fps_sprite->set_bitmap(bmp);

            last = SDL_GetPerformanceCounter();  //更新计时器
            count = 0;
        }
    }
    void destroy(){
        if(disposed)return;
        if(fps_sprite->bitmap) {
            fps_sprite->bitmap->dispose();
            delete fps_sprite->bitmap;
            delete fps_sprite;
        }
        disposed = true;
    }
};

class SceneStage1 : public Scene{   //STAGE1 的场景
    bool disposed;      //是否释放的标记
    Sprite *sp_back;    //背景图片的精灵

    FPS_Counter fps;   //fps记录
public:
    std::vector<std::function<void(void)> > tasks;                          //渲染任务队列

    std::map<std::pair<int, int>, std::function<void(void)> >key_handlers;  //按调-回调 表

    std::function<void(void)> logic_updater;                                //逻辑更新的函数
        //↑ 逻辑与渲染分离。

    Module *battle_module;    //战斗区域的模块
    virtual void start_scene(){    //启动场景。
        Scene::start_scene();

        //准备背景图片
        sp_back = new Sprite("sources/stage/back.png");
        main_module->manage(sp_back);

        //初始化战斗区域模块
        battle_module = new Module(25, 25, 400, 430);

        //启动fps记录
        fps.start();
        main_module->manage(fps.fps_sprite);

        disposed = false;
    }
    virtual void end_scene(){
        if(disposed)return;
        Scene::end_scene();

        //清除
        key_handlers.clear();
        tasks.clear();

        //释放精灵
        auto dispose = [&](Sprite *s){
            s->bitmap->dispose();
            delete s->bitmap;
            s->dispose();
            delete s;
        };
        dispose(sp_back);

        //释放模块
        delete battle_module;

        //fps counter
        fps.destroy();

        disposed = true;
    }
    virtual void update(){
        Scene::update();
        if (disposed)return;

        //处理请求队列的渲染任务
        for(auto &task : tasks)task();
        tasks.clear();

        //回调按键记录
        for(auto &handler : key_handlers){
            int key = handler.first.first;
            int state = handler.first.second;
            if(Input::key_is_state(key, state))
                handler.second();
            if(disposed)break;   //有可能遇到导致退出场景的回调
        }
        if (disposed)return;

        battle_module->update(); //更新战斗模块（模块会使得上面的精灵得到绘制）

       // draw_player_bomb();      //绘制残机数和炸弹数

        //上面的渲染做完了
        //更新逻辑
        logic_updater();

        //更新fps记录
        fps.addcnt();
    }
};

struct Player{              //玩家
    bool disposed;
    int dir;  //moving dir 0 stand 2 up 8 down 4 left 6 right. (10-dir)可以得到反方向
    Bitmap *player_map;             //玩家元件表
    Bitmap *player_bmp;             //玩家位图
    Sprite *sprite;                 //玩家的精灵
    SceneStage1 *scene;             //场景...其实可以SceneManager::scene的，但是不想毒瘤对吧...
    int x, y, speed;                //玩家的位置
    double SQRT2;

    static const int SPEED_FAST = 5;        //高速移动状态下的速度
    static const int SPEED_SLOW = 2;        //低俗移动状态下的速度

    void init(SceneStage1 *s){              //初始化角色
        scene = s;
        player_map = new Bitmap("sources/th14/player/pl00/pl00.png");
        player_bmp = new Bitmap(32, 48);  //素材的格式ZUN固定了...32x48的人物
        sprite = new Sprite(player_bmp);
        sprite->opacity = 0;
        x = 400/2-32/2, y = 430-48; SQRT2 = sqrt(2);        //初期位置
        speed = SPEED_FAST;

        scene->tasks.push_back([&](){  //请求战斗模块管理角色的精灵。
            scene->battle_module->manage(sprite);
            sprite->setpos(x, y);
            sprite->opacity = 255;
        });
        disposed = false;

        dir = 2;   //初期方向，朝上
        update();  //直接更新
    }
    void destroy(){  //销毁
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
        static int pos[10][2] = {   //人物朝向与在元件表里面位置对应关系
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
        player_bmp->clear();  //清除
        player_bmp->blt(HfRect(0, 0, 32, 48), player_map, HfRect(pos[dir][0], pos[dir][1], 32, 48));

        if(speed == SPEED_SLOW){  //绘制判定点
            player_bmp->fill_rect(32/2-8/2, 48/2-8/2, 8, 8, RGBA(0xff, 0xff, 0xff, 0xff));
            player_bmp->fill_rect(32/2-6/2, 48/2-6/2, 6, 6, RGBA(0xff, 0, 0, 0xff));
        }
        sprite->update();

        dir = 2;    //把朝向改回朝上（自然状态）
        #undef calc

        //speed
    }
    inline void pos_correct(){ //修正越界的位置并设置精灵位置
        if(x < 0)x = 0;
        if(y < 0)y = 0;
        if(x > 400-32)x = 400-32;
        if(y > 430-48)y = 430-48;
        sprite->setpos(x, y);
    }
    void on_keyup(){
        if(!Input::key_is_pressed(SDLK_UP))return;
        dir = 2;
        y -= (Input::key_is_pressed(SDLK_LEFT) || Input::key_is_pressed(SDLK_RIGHT))?(int)(1.0*speed/SQRT2):speed;
        pos_correct();
    }
    void on_keydown(){
        if(!Input::key_is_pressed(SDLK_DOWN))return;
        dir = 8;
        y += (Input::key_is_pressed(SDLK_LEFT) || Input::key_is_pressed(SDLK_RIGHT))?(int)(1.0*speed/SQRT2):speed;
        pos_correct();
    }
    void on_keyleft(){
        if(!Input::key_is_pressed(SDLK_LEFT))return;
        dir = 4;
        x -= (Input::key_is_pressed(SDLK_UP) || Input::key_is_pressed(SDLK_DOWN))?(int)(1.0*speed/SQRT2):speed;
        pos_correct();
    }
    void on_keyright(){
        if(!Input::key_is_pressed(SDLK_RIGHT))return;
        dir = 6;
        x += (Input::key_is_pressed(SDLK_UP) || Input::key_is_pressed(SDLK_DOWN))?(int)(1.0*speed/SQRT2):speed;
        pos_correct();
    }
    void on_keyshift(bool keyup = false){  //切换高低速状态
        if(keyup && speed == SPEED_SLOW)speed = SPEED_FAST;
        else if(!keyup)speed = SPEED_SLOW;
    }
};

struct PlayerShoot{    //玩家的子弹，子弹的攻击范围被设计为一个矩形
    bool disposed;      //释放的标记
    SceneStage1 *scene;   //其实可以....但是不要毒瘤对吧

    Bitmap *shoot_map;    //子弹元件表
    Bitmap *shoot_bmp;    //单个子弹的位图

    Bitmap *shoots_map;   //绘制好的，所有子弹
    Bitmap *shoots_bmp;   //这个根据需要，从shoots_map上blt
    Sprite *shoots_sprite; //shoots_bmp的精灵
    HfRect shoots_rect;    //子弹工具区域的矩形

    bool showing;  //是否显示子弹的标记

    std::function<std::pair<int, int>(void)> get_player_pos;  //其实可以...但是不要毒瘤对吧
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

        //子弹透明度变化和流动效果。
        static const int opasn = 9; //opacity chaging
        static int opas[opasn] = { 255, 240, 220, 190, 170, 150, 140, 120, 100 }; //玄学调参
        static int popas = 0;
        shoots_sprite->opacity = opas[(popas++)%opasn];
        shoots_bmp->blt_ex(HfRect(0, 0, 40, 16*27), shoots_map, HfRect(0, 0, 40, 16*27), 255, HfPoint(0, 0), 0, false, (popas%opasn)*12<54);

    }
};
struct PlayerBomb{  //角色大招
    bool disposed;
    Bitmap *light_bmp;  //中件白色的那个原盘
    Bitmap *mop_bmp;    //拖把的位图
    Bitmap *bomb_bmp;   //整个大招的位图
    Sprite *bomb_sprite;  //精灵↑
    SceneStage1 *scene;  //其实可以...对吧，但是不要毒瘤呢.
    bool showing;        //显示中的标记
    int show_time_left;  //显示的剩余时间
    std::function<std::pair<int, int>(void)> get_player_pos;  //其实可以...对吧，但是不要毒瘤呢
    std::map<std::pair<int, int>, std::function<void(void)> >::iterator pshift;  
                //在使用大招过程中，会强制进入低速模式，并且不会显示判定点，这里用了很简单的方法，就是调用shift被按下时候的handler

    static const int BOMB_WIDTH = 220;  //攻击范围宽度
    static const int BOMB_HEIGHT = 180; //攻击范围高度
    static const int BOMB_TIME = 60*7;  //持续时间（帧）
    static const int BOMB_FIBER = FIBER_USER+1;  //动画Fiber的编号
    static const int LIGHT_SIZE = 80;   //白色原盘的size...

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
        if(Fiber::fiber().exist(BOMB_FIBER))
            Fiber::fiber().destroy(BOMB_FIBER);

        disposed = true;
    }
    void show_fiber_proc(){
        int mop_angle = 0, mop_angle_sig = 1;
        int light_angle = 0;
                //没错上面都是局部变量
        while(show_time_left--){
            std::pair<int, int> player_pos = get_player_pos();
            int player_x = player_pos.first, player_y = player_pos.second;
            bomb_sprite->setpos(player_x+16-BOMB_WIDTH/2, player_y-BOMB_HEIGHT/2);

            //下面这些代码不要看，都是玄学二分法调参调出来的
            bomb_bmp->clear();
            bomb_bmp->blt_ex(HfRect((bomb_sprite->width()-LIGHT_SIZE)/2, (player_y-bomb_sprite->y())/2+LIGHT_SIZE/3, LIGHT_SIZE, LIGHT_SIZE),
                                 light_bmp, HfRect(0, 0, light_bmp->width(), light_bmp->height()),
                                 255,
                                 HfPoint(LIGHT_SIZE/2, LIGHT_SIZE/2), light_angle,
                                 false, false);

            bomb_bmp->blt_ex(HfRect(player_x-bomb_sprite->x()+LIGHT_SIZE/2+18, player_y-bomb_sprite->y()-90/2, 90, 90),
                                 mop_bmp, HfRect(0, 92, mop_bmp->width(), mop_bmp->height()-92),
                                 255,
                                 HfPoint(-LIGHT_SIZE/2, 90-30),
                                 mop_angle, false, false);

            if(mop_angle <= 0)mop_angle_sig = 1;
            else if(mop_angle >= 180)mop_angle_sig = -1;

            if(mop_angle <= 30 || mop_angle >= 150)mop_angle += mop_angle_sig*4;
            else mop_angle += mop_angle_sig*8;
            light_angle++;

            pshift->second();  //强制低速模式
            Fiber::fiber().yield();  //返回
        }
    }
    void show(){   //启动大招
        if(disposed || showing)return;

        showing = true;
        show_time_left = BOMB_TIME;
        Fiber::fiber()[BOMB_FIBER] = [&](){show_fiber_proc();};

    }
    void update(){  //更新大招的动画
        if(disposed)return;

        if(Fiber::fiber().exist(BOMB_FIBER) && Fiber::fiber().isHungUp(BOMB_FIBER)){
            if(show_time_left) //进入Fiber继续更新bomb动画
                Fiber::fiber().resume(BOMB_FIBER);
            else{       //动画显示完毕
                bomb_bmp->clear();
                showing = false;
                Fiber::fiber().destroy(BOMB_FIBER); //顺便干掉Fiber
            }
        }

    }
};

class STAGE1{               //STAGE1的逻辑，控制了游戏的流程等
    SceneStage1 *scene;
    Player player;
    PlayerShoot pshoot;
    PlayerBomb pbomb;
    int players;
    int bombs;

    void leave(){   //结束游戏
        player.destroy();
        pshoot.destroy();
        pbomb.destroy();
        scene->update_wait(20);
    }
public:
    void start(){  //启动
        SceneManager::call(scene = new SceneStage1);  //准备场景
        scene->logic_updater = [&,this](){update();}; //注册逻辑更新代码

        //注册ESC按钮
        scene->key_handlers[std::make_pair(SDLK_ESCAPE, Input::KEY_IS_TRIGGLED)] = [&,this](){leave(); SceneManager::jumpback();};

        player.init(scene); players = 2; //注册方向键和shift
        scene->key_handlers[std::make_pair(SDLK_UP, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyup();};
        scene->key_handlers[std::make_pair(SDLK_DOWN, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keydown();};
        scene->key_handlers[std::make_pair(SDLK_LEFT, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyleft();};
        scene->key_handlers[std::make_pair(SDLK_RIGHT, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyright();};
        scene->key_handlers[std::make_pair(SDLK_LSHIFT, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyshift(false);};
        scene->key_handlers[std::make_pair(SDLK_LSHIFT, Input::KEY_IS_UNPRESSED)] = [&,this](){player.on_keyshift(true);};

        pshoot.init(scene);        //注册射击按钮
        pshoot.get_player_pos = [&](){return std::make_pair(player.x, player.y);};
        scene->key_handlers[std::make_pair(SDLK_z,Input::KEY_IS_PRESSED)] = [&, this](){pressed_z();};
        scene->key_handlers[std::make_pair(SDLK_z, Input::KEY_IS_UNPRESSED)] = [&, this](){unpressed_z(); };

        pbomb.init(scene); bombs = 3;  //注册bomb按钮
        pbomb.get_player_pos = [&](){return std::make_pair(player.x, player.y);};
        scene->key_handlers[std::make_pair(SDLK_x, Input::KEY_IS_TRIGGLED)] = [&, this](){pbomb.show();};
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
