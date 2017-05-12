#include "Animation.h"
using namespace HfCloud;

Animation::Animation(){
    sprite = NULL;
    current_frame = 1;
    update_frame = [](){};
}
Animation::Animation(int fm):Animation(){
    total_frames = fm;
}
void Animation::update(){
    if(++current_frame > total_frames)return;
    update_frame();
}

