#include "Graphics.h"
#include "Bitmap.h"
using namespace HfCloud;

Bitmap::Bitmap(int w, int h){
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    texture = SDL_CreateTexture(Graphics::render, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, Graphics::width, Graphics::height);
    #else
    texture = SDL_CreateTexture(Graphics::render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Graphics::width, Graphics::height);
    #endif
    _w = w, _h = h;
}
Bitmap::Bitmap(const std::string &fn){
    texture = IMG_LoadTexture(Graphics::render, fn.c_str());
    SDL_QueryTexture(texture, NULL, NULL, &_w, &_h);
}
void Bitmap::dispose(){
    SDL_DestroyTexture(texture);
}
int Bitmap::width(){
    return _w;
}
int Bitmap::height(){
    return _h;
}

#define _SET_TARGET SDL_SetRenderTarget(Graphics::render, texture);
#define _SET_COLOR SDL_SetRenderDrawColor(Graphics::render, color.rgba.r, color.rgba.g, color.rgba.b, color.rgba.a);
#define _RESET_TARGET SDL_SetRenderTarget(Graphics::render, NULL);
void Bitmap::set_pixel(int x, int y, HfCloud::Color color){
    _SET_TARGET
    _SET_COLOR
    SDL_RenderDrawPoint(Graphics::render, x, y);
    _RESET_TARGET
}
void Bitmap::fill_rect(Hf_Rect &rect, HfCloud::Color color){
    _SET_TARGET
    _SET_COLOR
    SDL_RenderFillRect(Graphics::render, &rect);
    _RESET_TARGET
}
void Bitmap::fill_rect(int x, int y, int w, int h, HfCloud::Color color){
    //Hf_Rect rect = (Hf_Rect){x, y, w, h};   //ISO C++ forbids.
    Hf_Rect rect; rect.x = x, rect.y = y, rect.w = w, rect.h = h;
    fill_rect(rect, color);
}
void Bitmap::fill_rects(Hf_Rect *rs, HfCloud::Color color, int cnt){
    _SET_TARGET
    _SET_COLOR
    SDL_RenderFillRects(Graphics::render, rs, cnt);
    _RESET_TARGET
}
void Bitmap::set_pixels(Hf_Point *ps, HfCloud::Color color, int cnt){
    _SET_TARGET
    _SET_COLOR
    SDL_RenderDrawPoints(Graphics::render, ps, cnt);
    _RESET_TARGET
}
void Bitmap::clear(){
    _SET_TARGET
    _SET_COLOR
    SDL_SetRenderTarget(Graphics::render, texture);
    _RESET_TARGET
}

#undef _SET_TARGET
#undef _SET_COLOR
#undef _RESET_TARGET
