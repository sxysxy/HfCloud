#ifndef _SCENEMVC_H
#define _SCENEMVC_H

#include "stdinc.h"
#include "Scene.h"
#include "Input.h"

HFCLOUD_BEGIN

class SceneMVC : public Scene{
public:
    struct KeyEntry{                    /**< Key handler entry code.        */
        union{
            struct{
                int key, state;
            }info;                      /**< info stores the key and state. */
            long long hashcode;
        };
        KeyEntry(){}
        KeyEntry(int key, int state){  /**< constrcut from a key and state. */
            info.key = key;
            info.state = state;
        }
        KeyEntry(long long code){               /**< construct from code.   */
            hashcode = code;
        }
    };
#define make_keyentry(key, state) SceneMVC::KeyEntry(key, state)

private:
    std::vector<std::function<void(void)> > tasks;                          /**< tasks table.      */
    std::unordered_map<long long, std::function<void(void)> >key_handlers;  /**< key handlers map. */

public:
    std::function<void(void)> logic_updater;                                /**< logic controller code. */

    void register_key(const KeyEntry &entry, const std::function<void(void)> &handler);  /**< register a key_handler with a callable object. */
    void register_task(const std::function<void(void)> &task);                           /**< register a task. */

    virtual void update();
    virtual void start_scene();
    virtual void end_scene();
};


HFCLOUD_END

#endif // _SCENEMVC_H

