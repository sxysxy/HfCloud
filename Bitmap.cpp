#include "Graphics.h"
#include "Bitmap.h"

using namespace HfCloud;

Bitmap::Bitmap(int w, int h){
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    texture = SDL_CreateTexture(Graphics::render, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, w, h);
    #else
    texture = SDL_CreateTexture(Graphics::render, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    #endif
    HFASSERT_WITH_SDLERROR(texture != nullptr)

    _w = w, _h = h;
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}
Bitmap::Bitmap(const std::string &fn){
    texture = IMG_LoadTexture(Graphics::render, fn.c_str());
    HFASSERT_WITH_SDLERROR(texture != nullptr)

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
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
#define _RESET_COLOR SDL_SetRenderDrawColor(Graphics::render, 0, 0, 0, 0);
void Bitmap::set_pixel(int x, int y, HfCloud::Color color){
    _SET_TARGET
    _SET_COLOR
    SDL_RenderDrawPoint(Graphics::render, x, y);
    _RESET_TARGET
    _RESET_COLOR
}
void Bitmap::fill_rect(const HfRect &rect, HfCloud::Color color){
    _SET_TARGET
    _SET_COLOR
    SDL_RenderFillRect(Graphics::render, &rect);
    _RESET_TARGET
    _RESET_COLOR
}
void Bitmap::fill_rect(int x, int y, int w, int h, HfCloud::Color color){
    //Hf_Rect rect = (Hf_Rect){x, y, w, h};   //ISO C++ forbids.
    HfRect rect(x, y, w, h);
    fill_rect(rect, color);
}
void Bitmap::fill_rects(const HfRect *rs, HfCloud::Color color, int cnt){
    _SET_TARGET
    _SET_COLOR
    SDL_RenderFillRects(Graphics::render, rs, cnt);
    _RESET_TARGET
    _RESET_COLOR
}
void Bitmap::set_pixels(const HfPoint *ps, HfCloud::Color color, int cnt){
    _SET_TARGET
    _SET_COLOR
    SDL_RenderDrawPoints(Graphics::render, ps, cnt);
    _RESET_TARGET
    _RESET_COLOR
}
void Bitmap::clear(HfCloud::Color color){
    _SET_TARGET
    _SET_COLOR
    SDL_RenderClear(Graphics::render);
    _RESET_TARGET
    _RESET_COLOR
}
void Bitmap::clear(){
    clear(0);
}
void Bitmap::blt(const HfRect &dest, const Bitmap *bmp, const HfRect &src){
    _SET_TARGET
    SDL_RenderCopy(Graphics::render, bmp->texture, &src, &dest);
    _RESET_TARGET
}
void Bitmap::blt_ex(const HfRect &dest, const Bitmap *bmp, const HfRect &src, int opacity,
                    const HfPoint &renter, double angle, bool hmirror, bool vmirror){
    _SET_TARGET
    SDL_RendererFlip flip = (SDL_RendererFlip)((hmirror*SDL_FLIP_HORIZONTAL)|(vmirror*SDL_FLIP_VERTICAL));
    SDL_SetTextureAlphaMod(bmp->texture, opacity);
    SDL_RenderCopyEx(Graphics::render, bmp->texture, &src, &dest, -angle, &renter, flip);
    SDL_SetTextureAlphaMod(bmp->texture, 255);
    _RESET_TARGET
}

#undef _SET_TARGET
#undef _SET_COLOR
#undef _RESET_TARGET
