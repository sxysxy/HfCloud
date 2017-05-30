#include "stdinc.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Module.h"
using namespace HfCloud;

Sprite::Sprite(){
    _disposed = false;
    manager = NULL;
    bitmap = NULL;
    opacity = 255;
    angle = 0;
    vmirror = hmirror = false;
    rcenter.x = rcenter.y = 0;
    _x = show_rect.x = _y = show_rect.y = 0;
    scale_bitmap_height = scale_bitmap_width = 1.0;
}
Sprite::Sprite(const std::string &fn):Sprite(){
    set_bitmap(new Bitmap(fn));
}
Sprite::Sprite(const std::string &fn, int x, int y, int z = 0):Sprite(fn){
    setpos(x, y, z);
}
Sprite::Sprite(Bitmap *bmp):Sprite(){
    set_bitmap(bmp);
}
void Sprite::dispose(){
    _disposed = true;
}
void Sprite::set_bitmap(Bitmap *bmp){
    bitmap = bmp;
    set_origin_size();
    calc_rects();
}
void Sprite::update(){
    HFASSERT(!_disposed, "Update disposed sprite")
    if(_disposed)return;

    SDL_RendererFlip flip = (SDL_RendererFlip)((hmirror*SDL_FLIP_HORIZONTAL)|(vmirror*SDL_FLIP_VERTICAL));
    SDL_SetTextureAlphaMod(bitmap->texture, opacity);
    SDL_RenderCopyEx(Graphics::render, bitmap->texture, &clip_rect, &show_rect, -angle, &rcenter, flip);
}
#define _m_x manager->x()
#define _m_y manager->y()
#define _m_w manager->width()
#define _m_h manager->height()
void Sprite::calc_rects(){
    if(manager){
        SDL_Rect sprect, uni;
        sprect.x = _x+_m_x, sprect.y = _y+_m_y, sprect.w = width(), sprect.h = height();
        SDL_IntersectRect(&manager->rect, &sprect, &uni);
        show_rect = uni;
        clip_rect.x = sprect.x<uni.x?1.0*(uni.x-sprect.x)*scale_bitmap_width:0;
        clip_rect.y = sprect.y<uni.y?1.0*(uni.y-sprect.y)*scale_bitmap_height:0;
        clip_rect.w = show_rect.w*scale_bitmap_width;
        clip_rect.h = show_rect.h*scale_bitmap_height;
    }else{
        clip_rect.x = clip_rect.y = 0;
        clip_rect.w = _w*scale_bitmap_width, clip_rect.h = _h*scale_bitmap_height;
        show_rect.x = _x, show_rect.y = _y;
        show_rect.w = _w, show_rect.h = _h;
    }
}
#undef _m_x
#undef _m_y
#undef _m_w
#undef _m_h

void Sprite::setpos(int x, int y){
    _x = show_rect.x = x, _y = show_rect.y = y;
}
void Sprite::setpos(int x, int y, int z){
    setpos(x, y); setz(z);
}
void Sprite::setz(int z){
    ParamZ::setz(z); if(manager)manager->not_ordered();
}
void Sprite::scale_with_bitmap(){
    HFASSERT(_w != 0, "The width of a sprte can't be zero when scale_with_bitmap")
    HFASSERT(_h != 0, "The height of a sprte can't be zero when scale_with_bitmap")

    scale_bitmap_width = 1.0*bitmap->width()/_w;
    scale_bitmap_height = 1.0*bitmap->height()/_h;
}

