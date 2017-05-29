#ifndef _INPUT_H
#define _INPUT_H
#include "stdinc.h"

HFCLOUD_BEGIN

class Input{
public:
    static Uint8 *state_table;

    static void clear();                    /**<clear all vectors.*/
    static void update();                   /**<update by input events. */

    static bool key_is_pressed(int key);    /**<query that a kay is pressed?*/
};

HFCLOUD_END

#endif // _INPUT_H
