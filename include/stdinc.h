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

#ifdef _MSC_VER
#define HfText(s) s
#else
#define HfText(s) u8##s
#endif

#if defined(_MSC_VER) && !defined(HFDEBUG)
#ifdef _DEBUG
#define HFDEBUG
#endif
#endif

#ifdef HFDEBUG

#include <fstream>
namespace HfCloud{extern std::ofstream HfDebug_log;}
#define HFASSERT(x, info) \
    if(!(x)){HfCloud::HfDebug_log << "Assert Failed" << "   At:" << __FILE__ << " line: " <<__LINE__ << "\n    Error: " << info << '\n'; HfCloud::HfDebug_log.flush();}

#define HFASSERT_WITH_SDLERROR(x) HFASSERT(x, SDL_GetError())

#else

#define HFASSERT(x, info)
#define HFASSERT_WITH_SDLERROR(x)

#endif



#endif // _STD_INC_FILES
