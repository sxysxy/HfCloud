#include "Graphics.h"
#include "Bitmap.h"
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
    static std::function<void(void)> table[] = {
    [&](){
        static const long long tick = SDL_GetPerformanceFrequency()/60;
        static long long last = SDL_GetPerformanceCounter();
        long long now;
        while((now = SDL_GetPerformanceCounter())-last <= tick)std::this_thread::yield();
        SDL_RenderPresent(Graphics::render);
        last = SDL_GetPerformanceCounter();
    },
    [&](){
        SDL_RenderPresent(Graphics::render);}
    };
    table[Graphics::vsync]();
}
void Graphics::clear(){
    SDL_RenderClear(Graphics::render);
}
void Graphics::check_vsync(){
    #ifdef SDL_VIDEO_RENDER_D3D
    Graphics::vsync = 1;
    #elif defined(SDL_VIDEO_RENDER_D3D11)
    Graphics::vsync = 1;
    #elif defined(SDL_VIDEO_RENDER_OGL)
    SDL_GL_SetSwapInterval(1);
    if(SDL_GL_GetSwapInterval() > 0)
        Graphics::vsync = 1;
    #endif // defined
}
void Graphics::blend_color(HfCloud::Color color){
    SDL_SetRenderDrawColor(Graphics::render, color.rgba.r, color.rgba.g, color.rgba.b, color.rgba.a);
    HfRect h(0, 0, width, height);
    SDL_RenderFillRect(Graphics::render, &h);
    SDL_SetRenderDrawColor(Graphics::render, 0, 0, 0, 0);
}
