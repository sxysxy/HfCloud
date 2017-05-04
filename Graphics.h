#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "stdinc.h"
#include "Bitmap.h"

HFCLOUD_BEGIN

class Graphics{ /**< inclues Graphics functions and renderer functions, renderer functions renders fast(uses D3D9/OpenGL. */
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
    static void set_title(const std::string &t);  /**<set the title of the window. */

    static void set_render_color(HfCloud::Color color);  /**<set the color of then render when uses render's drawing functions*/
    static void fill_rect(Hf_Rect &rect);    /**<render a rect. */
    static void fill_rect(Hf_Rect &rect, HfCloud::Color color);
    static void draw_line(int x1, int y1, int x2, int y2);      /**<render a line */
    static void draw_line(int x1, int y1, int x2, int y2, HfCloud::Color color);
    static void set_pixel(int x, int y);     /**<render a point; */
    static void set_pixel(int x, int y, HfCloud::Color color);
    static void fill_rects(Hf_Rect *rects, int cnt);  /**<render some rects. */
    static void set_pixels(Hf_Point *ps, int cnt);    /**<redner some points. */

};

HFCLOUD_END

#endif //
