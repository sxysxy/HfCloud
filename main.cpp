#include "stdinc.h"
#include "SceneManager.h"
#include "Graphics.h"

#ifdef __WINDOWS__
#include "windows.h"
#endif // __WINDOWS__

using namespace HfCloud;
const int WINDOW_WIDTH = 544; /**<The width of the game window*/
const int WINDOW_HEIGHT = 416; /**<The height of the game window*/

struct Info{
    char title[256];
    SDL_Window *window;
    SDL_Renderer *render;
};

void ApplyConfig(Info &info){
    char buf[256];
    FILE *f = fopen("HfCloud.ini", "r");
    if(!f){
        printf("Error: Can not find the config file HfCloud.ini\n");
        exit(0);
    }
    while(fgets(buf, 256, f)){
        char *p1 = strchr(buf, '<');
        if(!p1)continue;
        if(!strncmp(p1+1, "title", 5)){
            p1 += 7;
            char *p2 = strchr(buf, '>');
            strncpy(info.title, p1, p2-p1);
        }
    }
}
int main(int argc, char *argv[]){
    Info info;
    ApplyConfig(info);

    SDL_Init(SDL_INIT_VIDEO);
    info.window = SDL_CreateWindow(info.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    WINDOW_WIDTH, WINDOW_HEIGHT,
                                    SDL_WINDOW_SHOWN);
    info.render = SDL_CreateRenderer(info.window , -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
                                                // open GPU accelerating, open vertical sync
    Graphics::window = info.window;
    Graphics::render = info.render;
    Graphics::width = WINDOW_WIDTH;
    Graphics::height = WINDOW_HEIGHT;

    #ifdef __WINDOWS__
    ConvertThreadToFiberEx(NULL, FIBER_FLAG_FLOAT_SWITCH);
    #endif // __WINDOWS__

    SceneManager::run(new SceneStart);

    if(Graphics::render)
        SDL_DestroyRenderer(Graphics::render);
    if(Graphics::window)
        SDL_DestroyWindow(Graphics::window);

    #ifdef __WINDOWS__
    ConvertFiberToThread();
    #endif // __WINDOWS__
    IMG_Quit();
    SDL_Quit();
    return 0;
}
