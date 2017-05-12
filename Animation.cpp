#include "Animation.h"
using namespace HfCloud;

Animation::Animation(){
    sprite = NULL;
    current_frame = 1;
    update_frame = [](){};
}
Animation::Animation(int fm):Animation(){
    frames = fm;
}
void Animation::update(){
    if(++current_frame > frames)return;
    update_frame();
}

