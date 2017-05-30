#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "stdinc.h"
#include "Bitmap.h"

HFCLOUD_BEGIN

class Graphics{ /**< inclues Graphics functions and renderer functions, renderer functions renders fast(uses D3D9/OpenGL. */
public:
                //some static vars
    static SDL_Window *window;    /**<the window                */
    static SDL_Renderer *render;  /**<the renderer of the window*/
    static int frame_count;       /**<frame count               */
    static int width;    /**<the width of the window(screen)    */
    static int height;   /**<the height of the window(screen)   */
    static int vsync;    /**<0 / 1 vsync not support / supprot  */

    static void resize(int w, int h);  /**<resize the window of a new (w, h)*/
    static void move_to(int x, int y); /**<move the window to (x, y)        */
    static void update();              /**<Graphics::update, very important but only has one line of code.*/
    static void clear();               /**<clear the Graphics                                             */
    static void set_title(const std::string &t);     /**<set the title of the window. */
    static void check_vsync();                       /**<check vsync is avaliable.    */

    static void blend_color(HfCloud::Color color);   /**<blend color on the screen.   */
};

HFCLOUD_END

#endif //
