#ifndef _MODULE_H
#define _MODULE_H

#include "stdinc.h"
#include "Sprite.h"
#include "Graphics.h"

HFCLOUD_BEGIN

class Module{
    bool _sp_ordered;           //oerderd flag
    void _sp_sort();            //sort the sprites.
public:
    Hf_Rect rect;                              /**<the rect shows the area it lays on*/
    std::vector<Sprite *> sprites;             /**<sprites*/

    Module(){
        _sp_ordered = true;
    }

    Module(int x, int y, int w, int h){
        Module();
        //rect = (Hf_Rect){x, y, w, h};     //ISO C++ forbids it...
        rect.x = x, rect.y = y, rect.w = w, rect.h = h;
    } /**<construct by a rect of (x, y, w, h)*/

    Module(Hf_Rect rt){
        Module();
        rect = rt;
    }; /**<construct by a rect*/

    virtual ~Module(){} /**<*/

    virtual void update(); /**<update all sprites it manages.*/

    void manage(Sprite *sp); /**<let the module manage sp.*/

    int x(){return rect.x;}
    int y(){return rect.y;}
    int width(){return rect.w;}
    int height(){return rect.h;}
    
    void not_ordered(){_sp_ordered = false;}
};

HFCLOUD_END

#endif // _MODULE_H
