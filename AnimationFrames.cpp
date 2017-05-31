#include "AnimationFrames.h"
using namespace HfCloud;

AnimationFrames::AnimationFrames(int fm):Animation(fm){
    update_frame = [&](Animation *ani){
        ani->sprite = ani_frames[current_frame]->sprite;
        if(ani->sprite)ani->sprite->update();

    };
    ani_frames.resize(fm);
    for(auto &a : ani_frames)a = nullptr;
}
