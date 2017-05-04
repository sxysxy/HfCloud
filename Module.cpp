#include "stdinc.h"
#include "Module.h"
using namespace HfCloud;

void Module::_sp_sort(){
    std::sort(sprites.begin(), sprites.end(), [](Sprite *a, Sprite *b){return *a < *b;});
                                                    // sort them by z prama.
    _sp_ordered = true;
}
void Module::update(){
    if(!_sp_ordered)_sp_sort();
    for(Sprite *s : sprites)s->update();
}
void Module::manage(Sprite *sp){
    sprites.push_back(sp);
    _sp_ordered = false;
}
