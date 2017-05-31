#ifndef _SPRITE_H
#define _SPRITE_H

#include "stdinc.h"
#include "ParamZ.h"
#include "Bitmap.h"

HFCLOUD_BEGIN
class Module; //declear it.
class Sprite : public ParamZ{  //Sprite
    bool _disposed;                 /**< disposed flag.                        */
    Sprite();                       /**< do not allow calling from outside.    */
    int _x, _y;                     /**< theoretical x, y of                   */
    int _w, _h;                     /**< theoretical w, h of                   */

public:
    Bitmap *bitmap;                 /**< the bitmap, you can draw on it                      */
    SDL_Rect show_rect;             /**< show on the screen in this rect.                    */
    SDL_Rect clip_rect;             /**< clip it in this rect.                               */
    Module *manager;                /**< managing module of this sprite, It's OK to be NULL. */
    unsigned char opacity;          /**< opacity value.                                      */
    double angle;                   /**< rotation angle. Anti-clockwise.                     */
    HfPoint rcenter;                /**< rotation center.                                    */
    bool hmirror;                   /**< horizon mirror flag.                                */
    bool vmirror;                   /**< vertical mirror flag.                               */
    double scale_bitmap_width;      /**< scale width of bitmap when clip it.                 */
    double scale_bitmap_height;     /**< scale height of bitmap hen clip it.                 */

public:
    virtual ~Sprite(){}     /**< useless, just let the g++ complier not to throw me warning.(delete polymorphic object)
                Do not use this destructor to free sources of a Sprite, It may cause faults sometimes.
                Please call dispose. */

    Sprite(const std::string &fn);                          /**< construct a Sprite from a file and initialize. */
    Sprite(const std::string &fn, int x, int y, int z);
    Sprite(Bitmap *bmp);                                    /**< construct a Sprite from a Bitmap               */
    void dispose();                                         /**< release the sources, then it can not be rendered.
                                                               NOTICE: it does not release bitmap it refrences. */
    void set_bitmap(Bitmap *bmp);                           /**<set the bitmap of this sprite.                  */
    void calc_rects();                                      /**< re_calc the rects                              */

    virtual void update();                                  /**< update it to show.                             */

    void setpos(int x, int y);                              /**< set the position of (x, y), do not affect z.   */
    void setpos(int x, int y, int z);                       /**< set the position of (x, y, z).                 */
    void setz(int z);
    inline int x(){return _x;}               /**< get x      */
    inline int y(){return _y;}               /**< get y      */
    inline int width(){return _w;}           /**< get width  */
    inline int height(){return _h;}          /**< get height */
    inline void zoom_width(double rate){_w = (int)(_w*rate);}        /**< zoom width.  */
    inline void zoom_height(double rate){_h = (int)(_h*rate);}       /**< zoom height. */
    inline void resize(int w, int h){_w = w, _h = h;}                /***< as its name.*/
    inline void set_origin_size(){resize(bitmap->width(), bitmap->height());}     /**< resize it with origin size. */
    void scale_with_bitmap();               /**< scale clip width and height with bitmap, then the Spriate can show the hole bitmap*/
    inline void scale_with_unit(){scale_bitmap_height = scale_bitmap_width = 1.0; }  /**< cancel clip width/height scaling .*/
};

HFCLOUD_END

#endif // _SPRITE_H
