#ifndef _INPUT_H
#define _INPUT_H
#include "stdinc.h"

HFCLOUD_BEGIN

struct KeyInfo{
    int sym, type;  //key symbol, event type
    bool repeat;    //repeat?
};
struct ButtonInfo{
    int type, clicks, button;  //event type, click times, button index.
    int x, y;       //position
};
class Input{
public:
    static std::vector<KeyInfo> keys;               /**<a std::vector stores key events*/
    static std::vector<ButtonInfo> buttons;         /**<a std::vector stores button events*/

    static void clear();                /**<clear all vectors.*/
    static void update(SDL_KeyboardEvent &keve);            /**<update by input events. */
    static void update(SDL_MouseButtonEvent &meve);         /**<update by button events. */

    static bool key_on_state(int key_sym, int key_state);   /**<query that a kay's state. */
    static bool key_is_triggled(int key);  /**<query that a kay is triggled?*/
};

HFCLOUD_END

#endif // _INPUT_H
