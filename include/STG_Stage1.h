#include "HfCloud.h"
#include "Font.h"
#include <unordered_map>
using namespace HfCloud;

struct FPS_Counter{  //FPS ��¼������addcnt����������£��ﵽ����֡��(ADVISED_FPS)��ʱ������һ��֡�ʵ����ݡ�
    bool disposed;
    long long last;
    long long now;
    long long feq;
    char buf[100];
    int count;
    Font font;
    Sprite *fps_sprite;
    static const int ADVISED_FPS = 60;
    void start(){                   //����
        feq = SDL_GetPerformanceFrequency();
        last = SDL_GetPerformanceCounter();
        count = 0;
        font.reload(u8"SIMYOU.TTF", 16);
        fps_sprite = new Sprite(new Bitmap(1, 1));      //���1,1��λͼ��û�õġ�
        fps_sprite->setpos(510, 430);               //FPS��ʾ��λ��
        disposed = false;
    }
    void addcnt(){
        if(++count == ADVISED_FPS){   //�ﵽ����
            fps_sprite->bitmap->dispose();
            delete fps_sprite->bitmap;

            now = SDL_GetPerformanceCounter();
            double sec = 1.0*(now - last) / feq;            //�������ϴδﵽ��ʱ���
            sprintf(buf, "FPS : %.2lf", ADVISED_FPS / sec); //�����֡��

            Bitmap *bmp = font.render_solid_text(buf, RGBA(0, 0xff, 0, 0xff));      //��������
            fps_sprite->set_bitmap(bmp);

            last = SDL_GetPerformanceCounter();  //���¼�ʱ��
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

class SceneStage1 : public Scene{   //STAGE1 �ĳ���
    bool disposed;      //�Ƿ��ͷŵı��
    Sprite *sp_back;    //����ͼƬ�ľ���

    FPS_Counter fps;   //fps��¼

    Sprite *sp_players, *sp_bombs;    //��ʾ�л���ը�������ľ���
public:
    std::vector<std::function<void(void)> > tasks;                          //��Ⱦ�������

    std::map<std::pair<int, int>, std::function<void(void)> >key_handlers;  //����-�ص� ��

    std::function<void(void)> logic_updater;                                //�߼����µĺ���
        //�� �߼�����Ⱦ���롣

    Module *battle_module;    //ս�������ģ��
    virtual void start_scene(){    //����������
        Scene::start_scene();

        //׼������ͼƬ
        sp_back = new Sprite("sources/stage/back.png");
        main_module->manage(sp_back);

        //��ʼ��ս������ģ��
        battle_module = new Module(25, 25, 400, 430);

        //����fps��¼
        fps.start();
        main_module->manage(fps.fps_sprite);

        disposed = false;
    }
    virtual void end_scene(){
        if(disposed)return;
        Scene::end_scene();

        //���
        key_handlers.clear();
        tasks.clear();

        //�ͷž���
        auto dispose = [&](Sprite *s){
            s->bitmap->dispose();
            delete s->bitmap;
            s->dispose();
            delete s;
        };
        dispose(sp_back);

        //�ͷ�ģ��
        delete battle_module;

        //fps counter
        fps.destroy();

        disposed = true;
    }
    virtual void update(){
        Scene::update();
        if (disposed)return;

        //����������е���Ⱦ����
        for(auto &task : tasks)task();
        tasks.clear();

        //�ص�������¼
        for(auto &handler : key_handlers){
            int key = handler.first.first;
            int state = handler.first.second;
            if(Input::key_is_state(key, state))
                handler.second();
            if(disposed)break;   //�п������������˳������Ļص�
        }
        if (disposed)return;

        battle_module->update(); //����ս��ģ�飨ģ���ʹ������ľ���õ����ƣ�

        //�������Ⱦ������
        //�����߼�
        logic_updater();

        //����fps��¼
        fps.addcnt();
    }
};

struct Player{              //���
    bool disposed;
    int dir;  //moving dir 0 stand 2 up 8 down 4 left 6 right. (10-dir)���Եõ�������
    Bitmap *player_map;             //���Ԫ����
    Bitmap *player_bmp;             //���λͼ
    Sprite *sprite;                 //��ҵľ���
    SceneStage1 *scene;             //����...��ʵ����SceneManager::scene�ģ����ǲ��붾���԰�...
    int x, y, speed;                //��ҵ�λ��
    double SQRT2;
    HfRect judge_pt;                //�ж���(λ����Խ�ɫ����ս��ģ��)

    int players, bombs;             //�л�������ը��������

    static const int SPEED_FAST = 5;        //�����ƶ�״̬�µ��ٶ�
    static const int SPEED_SLOW = 2;        //�����ƶ�״̬�µ��ٶ�

    void init(SceneStage1 *s){              //��ʼ����ɫ
        scene = s;
        player_map = new Bitmap("sources/th14/player/pl00/pl00.png");
        player_bmp = new Bitmap(32, 48);  //�زĵĸ�ʽZUN�̶���...32x48������
        sprite = new Sprite(player_bmp);
        sprite->opacity = 0;
        x = 400/2-32/2, y = 430-48; SQRT2 = sqrt(2);        //����λ��
        judge_pt = HfRect(32/2-6/2, 48/2-6/2, 6, 6);
        speed = SPEED_FAST;

        scene->tasks.push_back([&](){  //����ս��ģ������ɫ�ľ��顣
            scene->battle_module->manage(sprite);
            sprite->setpos(x, y);
            sprite->opacity = 255;
        });
        disposed = false;

        dir = 2;   //���ڷ��򣬳���
        update();  //ֱ�Ӹ���
    }
    void destroy(){  //����
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
        static int pos[10][2] = {   //���ﳯ������Ԫ��������λ�ö�Ӧ��ϵ
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
        player_bmp->clear();  //���
        player_bmp->blt(HfRect(0, 0, 32, 48), player_map, HfRect(pos[dir][0], pos[dir][1], 32, 48));

        if(speed == SPEED_SLOW){  //�����ж���
            player_bmp->fill_rect(32/2-8/2, 48/2-8/2, 8, 8, RGBA(0xff, 0xff, 0xff, 0xff));
            player_bmp->fill_rect(judge_pt, RGBA(0xff, 0, 0, 0xff));
        }
        sprite->update();

        dir = 2;    //�ѳ���Ļس��ϣ���Ȼ״̬��
        #undef calc

        //speed
    }
    inline void pos_correct(){ //����Խ���λ�ò����þ���λ��
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
    void on_keyshift(bool keyup = false){  //�л��ߵ���״̬
        if(keyup && speed == SPEED_SLOW)speed = SPEED_FAST;
        else if(!keyup)speed = SPEED_SLOW;
    }
};

struct PlayerShoot{    //��ҵ��ӵ����ӵ��Ĺ�����Χ�����Ϊһ������
    bool disposed;      //�ͷŵı��
    SceneStage1 *scene;   //��ʵ����....���ǲ�Ҫ�����԰�

    Bitmap *shoot_map;    //�ӵ�Ԫ����
    Bitmap *shoot_bmp;    //�����ӵ���λͼ

    Bitmap *shoots_map;   //���ƺõģ������ӵ�
    Bitmap *shoots_bmp;   //���������Ҫ����shoots_map��blt
    Sprite *shoots_sprite; //shoots_bmp�ľ���
    HfRect shoots_rect;    //�ӵ���������ľ���

    bool showing;  //�Ƿ���ʾ�ӵ��ı��

    std::function<std::pair<int, int>(void)> get_player_pos;  //��ʵ����...���ǲ�Ҫ�����԰�
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

        //�ӵ�͸���ȱ仯������Ч����
        static const int opasn = 9; //opacity chaging
        static int opas[opasn] = { 255, 240, 220, 190, 170, 150, 140, 120, 100 }; //��ѧ����
        static int popas = 0;
        shoots_sprite->opacity = opas[(popas++)%opasn];
        shoots_bmp->blt_ex(HfRect(0, 0, 40, 16*27), shoots_map, HfRect(0, 0, 40, 16*27), 255, HfPoint(0, 0), 0, false, (popas%opasn)*12<54);

    }
};
struct PlayerBomb{  //��ɫ����
    bool disposed;
    Bitmap *light_bmp;  //�м���ɫ���Ǹ�ԭ��
    Bitmap *mop_bmp;    //�ϰѵ�λͼ
    Bitmap *bomb_bmp;   //�������е�λͼ
    Sprite *bomb_sprite;  //�����
    SceneStage1 *scene;  //��ʵ����...�԰ɣ����ǲ�Ҫ������.
    bool showing;        //��ʾ�еı��
    int show_time_left;  //��ʾ��ʣ��ʱ��
    std::function<std::pair<int, int>(void)> get_player_pos;  //��ʵ����...�԰ɣ����ǲ�Ҫ������
    std::map<std::pair<int, int>, std::function<void(void)> >::iterator pshift;
                //��ʹ�ô��й����У���ǿ�ƽ������ģʽ�����Ҳ�����ʾ�ж��㣬�������˺ܼ򵥵ķ��������ǵ���shift������ʱ���handler

    static const int BOMB_WIDTH = 220;  //������Χ���
    static const int BOMB_HEIGHT = 180; //������Χ�߶�
    static const int BOMB_TIME = 60*7;  //����ʱ�䣨֡��
    static const int BOMB_FIBER = FIBER_USER+1;  //����Fiber�ı��
    static const int LIGHT_SIZE = 80;   //��ɫԭ�̵�size...

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

        pshift = scene->key_handlers.find(std::make_pair(SDLK_LSHIFT, Input::KEY_IS_PRESSED));  //�ҵ�����shift��.

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
                //û�����涼�Ǿֲ�����
        while(show_time_left--){
            std::pair<int, int> player_pos = get_player_pos();
            int player_x = player_pos.first, player_y = player_pos.second;
            bomb_sprite->setpos(player_x+16-BOMB_WIDTH/2, player_y-BOMB_HEIGHT/2);

            //������Щ���벻Ҫ����������ѧ���ַ����ε�������
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

            pshift->second();  //ǿ�Ƶ���ģʽ
            Fiber::fiber().yield();  //����
        }
    }
    void show(){   //��������
        if(disposed || showing)return;

        showing = true;
        show_time_left = BOMB_TIME;
        Fiber::fiber()[BOMB_FIBER] = [&](){show_fiber_proc();};

    }
    void update(){  //���´��еĶ���
        if(disposed)return;

        if(Fiber::fiber().exist(BOMB_FIBER) && Fiber::fiber().isHungUp(BOMB_FIBER)){
            if(show_time_left) //����Fiber��������bomb����
                Fiber::fiber().resume(BOMB_FIBER);
            else{       //������ʾ���
                bomb_bmp->clear();
                showing = false;
                Fiber::fiber().destroy(BOMB_FIBER); //˳��ɵ�Fiber
            }
        }

    }
};

class STAGE1{               //STAGE1���߼�����������Ϸ�����̵�
    SceneStage1 *scene;
    Player player;
    PlayerShoot pshoot;
    PlayerBomb pbomb;
    int players;
    int bombs;

    void leave(){   //������Ϸ
        player.destroy();
        pshoot.destroy();
        pbomb.destroy();
        scene->update_wait(20);
    }
public:
    void start(){  //����
        SceneManager::call(scene = new SceneStage1);  //׼������
        scene->logic_updater = [&,this](){update();}; //ע���߼����´���

        //ע��ESC��ť
        scene->key_handlers[std::make_pair(SDLK_ESCAPE, Input::KEY_IS_TRIGGLED)] = [&,this](){leave(); SceneManager::jumpback();};

        player.init(scene);         //ע�᷽�����shift
        scene->key_handlers[std::make_pair(SDLK_UP, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyup();};
        scene->key_handlers[std::make_pair(SDLK_DOWN, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keydown();};
        scene->key_handlers[std::make_pair(SDLK_LEFT, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyleft();};
        scene->key_handlers[std::make_pair(SDLK_RIGHT, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyright();};
        scene->key_handlers[std::make_pair(SDLK_LSHIFT, Input::KEY_IS_PRESSED)] = [&,this](){player.on_keyshift(false);};
        scene->key_handlers[std::make_pair(SDLK_LSHIFT, Input::KEY_IS_UNPRESSED)] = [&,this](){player.on_keyshift(true);};

        pshoot.init(scene);        //ע�������ť
        pshoot.get_player_pos = [&](){return std::make_pair(player.x, player.y);};
        scene->key_handlers[std::make_pair(SDLK_z,Input::KEY_IS_PRESSED)] = [&, this](){pressed_z();};
        scene->key_handlers[std::make_pair(SDLK_z, Input::KEY_IS_UNPRESSED)] = [&, this](){unpressed_z(); };

        pbomb.init(scene);         //ע��bomb��ť
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
