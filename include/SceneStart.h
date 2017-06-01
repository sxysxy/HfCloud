#ifndef _SCENESTART_H
#define _SCENESTART_H

#include "Graphics.h"
#include "Scene.h"
#include "SceneMVC.h"
using namespace HfCloud;
// the start scene
class SceneStart : public SceneMVC{
public:
    virtual void start_scene();     //when start the scene
    virtual void end_scene();       //and when end
};

#endif // _SCENESTART_H
