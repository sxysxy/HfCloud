#include "include/Graphics.h"
#include "include/Bitmap.h"
using namespace HfCloud;

void Graphics::resize(int w, int h){
    SDL_SetWindowSize(Graphics::window, w, h);
    Graphics::width = w, Graphics::height = h;
}

void Graphics::move_to(int x, int y){
    SDL_SetWindowPosition(Graphics::window, x, y);
}

void Graphics::set_title(const std::string &t){
    SDL_SetWindowTitle(Graphics::window, t.c_str());
}

void Graphics::update(){
    Graphics::frame_count++;
    SDL_RenderPresent(Graphics::render);
}
void Graphics::clear(){
    SDL_RenderClear(Graphics::render);
}
