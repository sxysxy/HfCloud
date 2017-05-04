#include "Input.h"
using namespace HfCloud;
std::vector<KeyInfo> Input::keys;
std::vector<ButtonInfo> Input::buttons;

void Input::clear(){
    Input::keys.clear();
    Input::buttons.clear();
}

void Input::update(SDL_KeyboardEvent &keve){
    KeyInfo info;
    info.repeat = keve.repeat;
    info.type = keve.type;
    info.sym = keve.keysym.sym;
    Input::keys.push_back(info);
}
void Input::update(SDL_MouseButtonEvent &meve){
    ButtonInfo info;
    info.clicks = meve.clicks;
    info.type = meve.type;
    info.x = meve.x;
    info.y = meve.y;
    info.button = meve.button;
    Input::buttons.push_back(info);
}
bool Input::key_is_triggled(int key_sym){
    for(KeyInfo &info : Input::keys)
        if(key_sym == info.sym && info.type == SDL_KEYDOWN)return true;
    return false;
}
