#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include "stdinc.h"
#include "Scene.h"
#include "SceneStart.h"

HFCLOUD_BEGIN

class SceneManager{
public:
    static Scene *scene;                    /**< current scene */
    static std::vector<Scene *> scenes;     /**< scenes stack  */

    static void run(Scene *scene);    /**< from here to go.*/
    static void exit();               /**< exit.           */
    static void clear();              /**< clear.          */

    static void jumpto(Scene *new_scene);       /**< jump to a scene, it can not jumpback. */
    static void call(Scene *enw_scene);         /**< call a scene, it can jumpback.        */
    static void jumpback();                     /**< return to prev_scene.                 */
};

HFCLOUD_END

#endif
