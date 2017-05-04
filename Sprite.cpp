#include "stdinc.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Module.h"
using namespace HfCloud;

inline Sprite::Sprite(){
    _disposed = false;
    manager = NULL;
    bitmap = NULL;
}
Sprite::Sprite(const std::string &fn):Sprite(){
    bitmap = new Bitmap(fn.c_str());
    texture = SDL_CreateTextureFromSurface(Graphics::render, bitmap->surface);
    calc_rects();
}
Sprite::Sprite(const std::string &fn, int x, int y, int z = 0):Sprite(fn){
    setpos(x, y, z);
}
Sprite::Sprite(Bitmap *bmp):Sprite(){
    bitmap = bmp;
    texture = SDL_CreateTextureFromSurface(Graphics::render, bitmap->surface);
    calc_rects();
}
Sprite::Sprite(SDL_Texture *tex){
    Sprite();
    texture = tex;
    calc_rects();
}
Sprite::Sprite(SDL_Texture *tex, int x, int y, int z = 0):Sprite(tex){
    setpos(x, y, z);
}
void Sprite::dispose(){
    if(bitmap){
        bitmap->dispose();
        bitmap = NULL;
    }
    if(texture){
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
    _disposed = true;
}
inline void Sprite::update(){
    if(_disposed)return;
    SDL_RenderCopy(Graphics::render, texture, &clip_rect, &show_rect);
}
void Sprite::calc_rects(){
    show_rect.x = manager?manager->x():0;
    show_rect.y = manager?manager->y():0;
    clip_rect.x = clip_rect.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &show_rect.w, &show_rect.h);
    clip_rect.w = show_rect.w, clip_rect.h = show_rect.h;
}
inline void Sprite::setpos(int x, int y){
    show_rect.x = x, show_rect.y = y;
}
inline void Sprite::setpos(int x, int y, int z){
    setpos(x, y); setz(z);
}
void Sprite::sync_texture(){
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTextureFromSurface(Graphics::render, bitmap->surface);
}
