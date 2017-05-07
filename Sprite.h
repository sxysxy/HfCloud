#ifndef _SPRITE_H
#define _SPRITE_H

#include "stdinc.h"
#include "ParamZ.h"
#include "Bitmap.h"

HFCLOUD_BEGIN
class Module; //declear it.
class Sprite : public ParamZ{  //Sprite
    bool _disposed;         /**< disposed flag. */
public:
    Bitmap *bitmap;         /**< the bitmap, you can edit it, then sync_texture */
    SDL_Rect show_rect;     /**< show on the screen in this rect. */
    SDL_Rect clip_rect;     /**< clip it in this rect. */
    Module *manager;        /**< managing module of this sprite, It's OK to be NULL. */
    unsigned char opacity;  /**< opacity value. */

    Sprite();
    virtual ~Sprite(){}     /**< useless, just let the g++ complier not to throw me warning.(delete polymorphic object)
                Do not use this destructor to free sources of a Sprite, It may cause faults sometimes.
                Please call dispose. */

    Sprite(const std::string &fn);  /**< construct a Sprite from a file and initialize. */
    Sprite(const std::string &fn, int x, int y, int z);
    Sprite(Bitmap *bmp);    /**< construct a Sprite from a Bitmap*/
    void dispose();         /**< release the sources, then it can not be rendered.
                                NOTICE: it does not release bitmap it refrences. */
    void calc_rects();      /**< re_calc the rects */

    virtual void update();          /**< update it to show. */

    void setpos(int x, int y);               /**< set the position of (x, y), do not affect z. */
    void setpos(int x, int y, int z);        /**< set the position of (x, y, z). */
    void setz(int z);
    inline int x(){return show_rect.x;}            /**< get x */
    inline int y(){return show_rect.y;}            /**< get y */
    inline int width(){return show_rect.w;}        /**< get width */
    inline int height(){return show_rect.h;}       /**< get height */
};

HFCLOUD_END

#endif // _SPRITE_H
