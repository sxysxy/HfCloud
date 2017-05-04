#include "Graphics.h"
using namespace HfCloud;

SDL_Window *Graphics::window;
SDL_Renderer *Graphics::render;
int Graphics::frame_count = 0;
int Graphics::width, Graphics::height;

inline
void Graphics::resize(int w, int h){
    SDL_SetWindowSize(Graphics::window, w, h);
    Graphics::width = w, Graphics::height = h;
}

inline
void Graphics::move_to(int x, int y){
    SDL_SetWindowPosition(Graphics::window, x, y);
}

void Graphics::update(){
    SDL_RenderPresent(Graphics::render);
    Graphics::frame_count++;
}
void Graphics::clear(){
    SDL_RenderClear(Graphics::render);
}
