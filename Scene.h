#ifndef _SCENE_H
#define _SCENE_H
#include "Module.h"

HFCLOUD_BEGIN

class Scene{
public:
    Module *main_module;                /**<main module of this scene, it can help manage sprites.*/
    virtual void start_scene();         /**<when start the scene.*/
    virtual void end_scene();           /**<when end.*/
    virtual void main_proc();           /**<main proc. why don't use 'main' as name? because of 'SDL_main' ...*/
    virtual void update_event(bool);    /**<events update.*/
    virtual void update();              /**<update proc. It should call Graphics::update after it. */
    void update_for_wait();             /**<update only for wait a frame. */
    void wait(int d);                   /**<wait for d frames. */
    Scene();
    virtual ~Scene();
}; //

HFCLOUD_END

#endif // _SCENE_H
