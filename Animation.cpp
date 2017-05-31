#include "Animation.h"
using namespace HfCloud;

Animation::Animation(){
    sprite = NULL;
    current_frame = 0;
    update_frame = [](Animation *ani){};
}
Animation::Animation(int fm):Animation(){
    HFASSERT(fm >= 0, "Total frames of an Animation should >= 0")
    total_frames = fm;
}
void Animation::update(){
    if(current_frame >= total_frames)return;
    update_frame(this);
    current_frame++;   //
}

