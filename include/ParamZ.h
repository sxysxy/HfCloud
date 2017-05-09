#ifndef _PARAMZ_H
#define _PARAMZ_H

HFCLOUD_BEGIN

// z param.

class ParamZ{
    int _z;
public:
    ParamZ(){_z = 0;}
    inline void setz(int z_){_z = z_;} /**< set the z dimension */
    inline int z()const {return _z;}   /**< get z */
    bool operator<(const ParamZ &p)const{  //sort by z
        return z() < p.z();
    }
};

HFCLOUD_END

#endif // _PARAMZ_H
