#ifndef _ANIMATION_FRAMES_H
#define _ANIMATION_FRAMES_H

#include "stdinc.h"
#include "Sprite.h"
#include "Animation.h"

HFCLOUD_BEGIN

struct AnimationFrame{
    Sprite *sprite;             //Sprite of this frame

                    //features in future: se(sound effetion),...etc
    AnimationFrame(Sprite *s = nullptr){sprite = s;}
};

class AnimationFrames : public Animation{
public:
    std::vector<AnimationFrame *> ani_frames;

    AnimationFrames(int fm);
};

HFCLOUD_END

#endif
