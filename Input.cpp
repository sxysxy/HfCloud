#include "Input.h"
using namespace HfCloud;

void Input::update(){
    SDL_PumpEvents();
}
bool Input::key_is_pressed(int key_sym){
    return state_table[SDL_GetScancodeFromKey(key_sym)];
}
