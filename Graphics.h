#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "stdinc.h"

HFCLOUD_BEGIN

class Graphics{
public:
                //some static vars
    static SDL_Window *window;  /**<the window*/
    static SDL_Renderer *render;  /**<the renderer of the window*/
    static int frame_count;     /**<frame count*/
    static int width;    /**<the width of the window(screen)*/
    static int height;   /**<the height of the window(screen)*/

    static void resize(int w, int h);  /**<resize the window of a new (w, h)*/
    static void move_to(int x, int y); /**<move the window to (x, y)*/
    static void update();  /**<Graphics::update, very important but online has one line of code.*/
    static void clear();   /**<clear the Graphics*/
};

HFCLOUD_END

#endif //
