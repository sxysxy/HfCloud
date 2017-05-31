#include "Input.h"
using namespace HfCloud;

void Input::update(){
    memcpy(state_last, state_table, sizeof(Uint8)*table_size);
    SDL_PumpEvents();
}
bool Input::key_is_pressed(int key_sym){
    return state_table[SDL_GetScancodeFromKey(key_sym)];
}
bool Input::key_is_triggled(int key_sym){
    SDL_Scancode sc = SDL_GetScancodeFromKey(key_sym);
    return !state_last[sc] && state_table[sc];
}
bool Input::key_is_released(int key_sym){
    SDL_Scancode sc = SDL_GetScancodeFromKey(key_sym);
    return state_last[sc] && !state_table[sc];
}

void Input::Input_Quit(){
    if(state_last)free(state_last);
}
