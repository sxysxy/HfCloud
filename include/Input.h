#ifndef _INPUT_H
#define _INPUT_H
#include "stdinc.h"

HFCLOUD_BEGIN

class Input{
public:
    static Uint8 *state_table;
    static Uint8 *state_last;
    static int table_size;

    static void clear();                        /**<clear all vectors.*/
    static void update();                       /**<update by input events. */

    static bool key_is_pressed(int key);        /**<query that a kay is pressed?                        */
    static bool key_is_triggled(int key_sym);   /**<query that a key becomes pressed from unpressed.    */
    static bool key_is_released(int key_sym);   /**<query that a key becomes unpressed from pressed.    */


    static void Input_Quit();
};

HFCLOUD_END

#endif // _INPUT_H
