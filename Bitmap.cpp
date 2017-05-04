#include "Bitmap.h"
using namespace HfCloud;

Bitmap::Bitmap(int w, int h){
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    surface = SDL_CreateRGBSurface(0, w, h, 32, 0xff000000u, 0x00ff0000u, 0x0000ff00u, 0x000000ffu);
    #else
    surface = SDL_CreateRGBSurface(0, w, h, 32, 0x000000ffu, 0x0000ff00u, 0x00ff0000u, 0xff000000u);
    #endif
}
Bitmap::Bitmap(const std::string &fn){
    surface = IMG_Load(fn.c_str());
}
void Bitmap::dispose(){
    SDL_FreeSurface(surface);
}
int Bitmap::width(){
    return surface->w;
}
int Bitmap::height(){
    return surface->h;
}
void Bitmap::set_pixel(int x, int y, HfCloud::Color color){
    *(HfCloud::Color*)(y*width()+x) = color;
}
void Bitmap::fill_rect(Hf_Rect &rect, HfCloud::Color color){
    SDL_FillRect(surface, &rect, color.color);
}
void Bitmap::fill_rects(Hf_Rect *rs, HfCloud::Color color, int cnt){
    SDL_FillRects(surface, rs, cnt, color.color);
}
void Bitmap::set_pixels(Hf_Point *ps, HfCloud::Color color, int cnt){
    for(int i = 0; i < cnt; i++)set_pixel(ps[i].x, ps[i].y, color);
}
void Bitmap::clear(){
    memset(surface->pixels, 0, 4*width()*height());
}
