#include "Input.h"
using namespace HfCloud;

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
bool Input::key_on_state(int key_sym, int key_state){
    for(KeyInfo &info : Input::keys)
        if(key_sym == info.sym && info.type == key_state)return true;
    return false;
}
bool Input::key_is_triggled(int key_sym){
    return key_on_state(key_sym, SDL_KEYDOWN);
}
