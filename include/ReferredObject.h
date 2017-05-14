#ifndef _REFERREDOBJECT_H
#define _REFERREDOBJECT_H

class ReferredObject{
protected:
    int referred_counter;
    bool released;
public:
    ReferredObject(){
        referred_counter = 0;
    }
    virtual ~ReferredObject(){}
    
};

#endif
