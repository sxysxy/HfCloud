#include "Graphics.h"
#include "Bitmap.h"
using namespace HfCloud;

SDL_Window *Graphics::window;
SDL_Renderer *Graphics::render;
int Graphics::frame_count = 0;
int Graphics::width, Graphics::height;

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
void Graphics::set_render_color(HfCloud::Color color){
    SDL_SetRenderDrawColor(Graphics::render, color.rgba.r, color.rgba.g, color.rgba.b, color.rgba.a);
}
void Graphics::fill_rect(Hf_Rect &rect){
    SDL_RenderDrawRect(Graphics::render, &rect);
}
void Graphics::fill_rect(Hf_Rect &rect, HfCloud::Color color){
    set_render_color(color);
    fill_rect(rect);
}
void Graphics::draw_line(int x1, int y1, int x2, int y2){
    SDL_RenderDrawLine(Graphics::render, x1, y1, x2, y2);
}
void Graphics::draw_line(int x1, int y1, int x2, int y2, HfCloud::Color color){
    set_render_color(color);
    draw_line(x1, y1, x2, y2);
}
void Graphics::set_pixel(int x, int y){
    SDL_RenderDrawPoint(Graphics::render, x, y);
}
void Graphics::set_pixel(int x, int y, HfCloud::Color color){
    set_render_color(color);
    set_pixel(x, y);
}
void Graphics::set_pixels(Hf_Point *ps, int cnt){
    SDL_RenderDrawPoints(Graphics::render, ps, cnt);
}
void Graphics::fill_rects(Hf_Rect *rs, int cnt){
    SDL_RenderDrawRects(Graphics::render, rs, cnt);
}

