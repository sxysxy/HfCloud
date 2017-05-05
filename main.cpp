#include "stdinc.h"
#include "SceneManager.h"
#include "Graphics.h"
//Main win32
#include <windows.h>
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
        MessageBox(0, "Can not find the config file HfCloud.ini", "Error", 0);
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
    info.window = SDL_CreateWindow(info.title, GetSystemMetrics(SM_CXSCREEN)/2-WINDOW_WIDTH/2,
                                    GetSystemMetrics(SM_CYSCREEN)/2-WINDOW_HEIGHT/2,
                                    WINDOW_WIDTH, WINDOW_HEIGHT,
                                    SDL_WINDOW_SHOWN);
    info.render = SDL_CreateRenderer(info.window , -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
                                                // open GPU accelerating, open vertical sync
    Graphics::window = info.window;
    Graphics::render = info.render;
    Graphics::width = WINDOW_WIDTH;
    Graphics::height = WINDOW_HEIGHT;

    SceneManager::run(new SceneStart);

    if(Graphics::render)
        SDL_DestroyRenderer(Graphics::render);
    if(Graphics::window)
        SDL_DestroyWindow(Graphics::window);

    IMG_Quit();
    SDL_Quit();
    return 0;
}
