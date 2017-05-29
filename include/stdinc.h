#ifndef _STD_INC_FILES
#define _STD_INC_FILES

// includes some headers.

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <set>
#include <list>
#include <queue>
#include <deque>
#include <vector>
#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <SDL.h>
#include <SDL_image.h>

namespace HfCloud{}

#define HFCLOUD_BEGIN namespace HfCloud{
#define HFCLOUD_END }

namespace HfCloud{
struct HfRect : public SDL_Rect{
    HfRect(int _x = 0, int _y = 0, int _w = 0, int _h = 0){
        x = _x, y = _y, w = _w, h = _h;
    }
};
struct HfPoint : public SDL_Point{
    HfPoint(int _x = 0, int _y = 0){
        x = _x, y = _y;
    }
};
}
#endif // _STD_INC_FILES
