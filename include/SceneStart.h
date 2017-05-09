#ifndef _SCENESTART_H
#define _SCENESTART_H

#include "Graphics.h"
#include "Scene.h"
using namespace HfCloud;
// the start scene
class SceneStart : public Scene{
public:
    virtual void start_scene();     //when start the scene
    virtual void end_scene();       //and when end
    virtual void update();          //update
};

#endif // _SCENESTART_H
