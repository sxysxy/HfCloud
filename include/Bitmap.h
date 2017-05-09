#ifndef _BITMAP_H
#define _BITMAP_H

#include "stdinc.h"

HFCLOUD_BEGIN

struct Color{
    union{
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        struct{
            unsigned char a:8;
            unsigned char b:8;
            unsigned char g:8;
            unsigned char r:8;
        }rgba;
        #else
        struct{
            unsigned char r:8;
            unsigned char g:8;
            unsigned char b:8;
            unsigned char a:8;
        }rgba; //According to ISO C++ balabala.....
        #endif
        unsigned int color;
    };
    Color(const unsigned c = 0){color = c;}
};
inline unsigned RGBA(unsigned r, unsigned g, unsigned b, unsigned a){
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    return (r<<24) | (g<<16) | (b<<8) | a;
    #else
    return (a<<24) | (b<<16) | (g<<8) | r;
    #endif
}
class Bitmap{
    int _w;     /**< width, private, only provide reading interface */
    int _h;     /**< height, private, only provide reading interface */
public:
    SDL_Texture *texture;       /**<SDL_Texture Object of this Bitmap object */
    HfCloud::Color color;       /**<Current Color. */
    Bitmap(int w, int h);       /**<Create a bitmap of (width:w, height:h). RGBA Model.  */
    Bitmap(const std::string &fn);   /**<Create a bitmap from a picture file. */
    ~Bitmap(){}                      /**<useless, as for Bitmap, DO NOT FREE SOURCES IN DESTRUCTOR.
                                                It may cause fatal errors. */
    void dispose();             /**<dispose the sources */

    int width();            /**<get the width of the bitmap*/
    int height();           /**<get the height of the bitmap*/
    void fill_rect(Hf_Rect &rect, HfCloud::Color color); /**<render a rect. */
    void fill_rect(int x, int y, int w, int h, HfCloud::Color color); /**<render a rect. */
    void fill_rects(Hf_Rect *rects, HfCloud::Color color, int cnt);  /**<render some rects. */
    void set_pixel(int x, int y, HfCloud::Color color);  /**<render a point; */
    void set_pixels(Hf_Point *ps, HfCloud::Color color, int cnt);    /**<redner some points. */
    void clear();       /**<clear this bitmap*/
};
HFCLOUD_END
#endif // _BITMAP_H
