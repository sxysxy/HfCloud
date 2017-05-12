#ifndef _ANIMATION_FRAMES_H
#define _ANIMATION_FRAMES_H

#include "stdinc.h"
#include "Sprite.h"
#include "Animation.h"

HFCLOUD_BEGIN

struct AnimationFrame{
    Sprite *sprite;

};

class AnimationFrames : public Animation{

};

HFCLOUD_END

#endif
