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
}
Sprite::Sprite(const std::string &fn):Sprite(){
    bitmap = new Bitmap(fn);
    calc_rects();
}
Sprite::Sprite(const std::string &fn, int x, int y, int z = 0):Sprite(fn){
    setpos(x, y, z);
}
Sprite::Sprite(Bitmap *bmp):Sprite(){
    bitmap = bmp;
    calc_rects();
}
void Sprite::dispose(){
    _disposed = true;
}
void Sprite::update(){
    if(_disposed)return;
    SDL_RenderCopy(Graphics::render, bitmap->texture, &clip_rect, &show_rect);
}
void Sprite::calc_rects(){
    show_rect.x = manager?manager->x():0;
    show_rect.y = manager?manager->y():0;
    clip_rect.x = clip_rect.y = 0;
    show_rect.w = bitmap->width(); show_rect.h = bitmap->height();
    clip_rect.w = show_rect.w, clip_rect.h = show_rect.h;
}
void Sprite::setpos(int x, int y){
    show_rect.x = x, show_rect.y = y;
}
void Sprite::setpos(int x, int y, int z){
    setpos(x, y); setz(z);
}
void Sprite::setz(int z){
  ParamZ::setz(z); if(manager)manager->not_ordered();
}
