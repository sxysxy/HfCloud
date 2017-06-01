#include "stdinc.h"

#ifdef _MSC_VER

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_ttf.lib")

#endif

#ifdef __MINGW32_MAJOR_VERSION
        //mingw environment...
        //ld : undefined refrence to 'SDL_ceil', undefined refrence to ..... WTF???
        //then I write these functions...
double SDLCALL SDL_ceil(double x){
    return ceil(x);
}
size_t SDLCALL SDL_strlen(const char *s){
    return strlen(s);
}
char *SDLCALL SDL_strchr(const char *s, int c){
    return strchr(s, c);
}
size_t SDLCALL SDL_strlcpy(char *dst, const char *src, size_t maxlen){
    for(int i = 0; i < maxlen-1 && src[i]; i++)
        dst[i] = src[i];
    dst[maxlen-1] = 0;
    return strlen(src);
}

#endif
