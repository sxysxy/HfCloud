#ifndef _SCENE_H
#define _SCENE_H
#include "stdinc.h"
#include "Module.h"
#include "Fiber.h"
HFCLOUD_BEGIN

class Scene{
    Fiber *fiber;                       /**<scene fiber. */
  //  bool fading;
public:
    int fade_time;
    Module *main_module;                /**<main module of this scene, it can help manage sprites.      */
    virtual void start_scene();         /**<when start the scene.                                       */
    virtual void end_scene();           /**<when end.                                                   */
    virtual void main_proc();           /**<main proc. why don't use 'main' as name? because of 'SDL_main' ...*/
    virtual void update_event();        /**<events update.                                                    */
    virtual void update();              /**<update proc. It should call Graphics::update after it.            */
    void update_for_wait();             /**<update only for wait a frame.                                     */
    void update_wait(int d);            /**<update_wait for d frames.                                                      */
    void update_wait(int d, const std::function<void(int)> &updater);  /**<update_wait with a lambda called by each frame. */
    void yield_for_wait();              /**<return from fiber to wait a frame.                  */
    void wait(int d);                   /**<yield_for_wait d frame.                             */
    void wait(int d, const std::function<void(int)> &updater); /**<wait with a lambda.          */

    Scene();
    virtual ~Scene();
  //  void fade_in();
  //  void fade_out();
}; //

HFCLOUD_END

#endif // _SCENE_H
