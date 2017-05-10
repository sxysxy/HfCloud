#include "include/stdinc.h"
#include "include/Sprite.h"
#include "include/Graphics.h"
#include "include/Module.h"
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
    SDL_SetTextureBlendMode(bitmap->texture, SDL_BLENDMODE_BLEND);
}
void Sprite::update(){
    if(_disposed)return;
    SDL_RendererFlip flip = (SDL_RendererFlip)((hmirror*SDL_FLIP_HORIZONTAL)|(vmirror*SDL_FLIP_VERTICAL));
    SDL_SetTextureAlphaMod(bitmap->texture, opacity);
    SDL_RenderCopyEx(Graphics::render, bitmap->texture, &clip_rect, &show_rect, -angle, &rcenter, flip);
}
#define _m_x() manager->x()
#define _m_y() manager->y()
#define _m_w() manager->width()
#define _m_h() manager->height()
void Sprite::calc_rects(){
    if(manager){
        if(x() > _m_x()+_m_w() || x()+width() < _m_x() ||
                    y() > _m_y()+_m_h() || y()+height() < _m_y()){
            clip_rect.w = clip_rect.h = 0; return;     // outside manager's rect
        }
        int x1_ = std::min(_m_x(), _x), x2_ = std::max(_m_x()+_m_w(), _x+_w);
        int y1_ = std::min(_m_y(), _y), y2_ = std::max(_m_y()+_m_h(), _y+_h);

        int x1 = _m_x() == x1_ ? _x : _m_x();
        int x2 = _m_x()+_m_w() == x2_ ? _x+_w : _m_x()+_m_w();
        int y1 = _m_y() == y1_ ? _y : _m_y();
        int y2 = _m_y()+_m_h() == y2_ ? _y+_h : _m_y()+_m_h();

        show_rect.x = x1, show_rect.w = x2-x1;
        show_rect.y = y1, show_rect.h = y2-y1;
        clip_rect.x = std::max(0, x1)/scale_bitmap_width; clip_rect.w = show_rect.w * scale_bitmap_width;
        clip_rect.y = std::max(0, y1)/scale_bitmap_height; clip_rect.h = show_rect.h * scale_bitmap_height;
    }else{
        clip_rect.x = clip_rect.y = 0;
        clip_rect.w = _w*scale_bitmap_width, clip_rect.h = _h*scale_bitmap_height;
        show_rect.x = _x, show_rect.y = _y;
        show_rect.w = _h, show_rect.h = _h;
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
    scale_bitmap_width = 1.0*bitmap->width()/_w;
    scale_bitmap_height = 1.0*bitmap->height()/_h;
}

