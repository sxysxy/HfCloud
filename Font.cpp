#include "Font.h"
#include "Graphics.h"

HFCLOUD_BEGIN

void Font::_constrcut(const std::string &fn, int size){
    _font = TTF_OpenFont(fn.c_str(), size);
    HFASSERT_WITH_SDLERROR(_font != nullptr);

    _filename = fn;
    _size = size;
    _style = TTF_STYLE_NORMAL;
}
void Font::_destruct(){
    if(_font)
        TTF_CloseFont(_font);
    _font = nullptr;
}
Font::Font(){
    _font = nullptr;
}
Font::Font(const std::string &fn, int size):Font(){
    _constrcut(fn, size);
}
void Font::reload(const std::string &fn){
    _destruct();
    _constrcut(fn, _size);
}
void Font::reload(int size){
    _destruct();
    _constrcut(_filename, size);
}
void Font::reload(const std::string &fn, int size){
    _destruct();
    _constrcut(fn, size);
}
void Font::set_style(int nstyle){
    TTF_SetFontStyle(_font, nstyle);
    _style = nstyle;
}

Bitmap *Font::render_solid_text(const std::string &text, HfCloud::Color color){
    SDL_Surface *s = TTF_RenderText_Solid(_font, text.c_str(), HfColor(color));
    HFASSERT(s != nullptr, TTF_GetError());
    SDL_Texture *tex = SDL_CreateTextureFromSurface(Graphics::render, s);
    HFASSERT_WITH_SDLERROR(tex != nullptr)
    return new Bitmap(tex);
}

HFCLOUD_END
