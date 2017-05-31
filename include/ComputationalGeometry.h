#include "stdinc.h"

/*

http://cogs.pro/cogs/submit/code.php?id=250411 thanks to him...
thanks to RuJia Liu's book.

*/

HFCLOUD_BEGIN

const double CG_PI = acos(-1);
const double CG_2PI = 2*acos(-1);
const double CG_EPS = 1e-8;
inline int CGdcmp(double x){ /**< returns the sig of x*/
    static int retval[2][2] = {{0, 1}, {-1, 0}};
    return retval[x<-CG_EPS][x>CG_EPS];
}
struct CGPoint;
typedef CGPoint CGVector;

struct CGPoint{
    double x, y;
    CGPoint(){x = y = 0;}
    CGPoint(double _x, double _y):x(_x), y(_y){};
    CGPoint(double len, double angle, bool anticlock){ /**< using length, angle, anticloc to initialize position. */
        x = len*(anticlock?cos(angle):cos(CG_2PI-angle));
        y = len*(anticlock?sin(angle):sin(CG_2PI-angle));
    }
    double length();                        /**< get the lenght of (0,0)->(x,y). */
    double angle();                         /**< get the point's angle (0,0)->(x,y), return-value is between [0, 2PI).  */
    CGPoint& unit();                        /**< get the unit vector of (0,0)->(x,y). (modify itself)                   */
    CGPoint& rotate(double angle);          /**< rotate the vector of (0,0)->(x,y) with a angle, anticlock.             */
    CGPoint& setlength(double len);         /**< reset the length of the vector(0. 0)->(x, y), when its length > 0.     */

    bool operator==(const CGPoint &p2)const;                /**<judge that p1 == p2.      */
    bool operator!=(const CGPoint &p2)const;                /**<judge that p1 != p2.      */
    CGVector operator+(const CGVector &v2)const;            /**<vector addition.          */
    CGVector operator-(const CGPoint &p2)const;             /**<p1-p2 = a vector.         */
    CGVector operator*(double p)const;                      /**<vector mul a real.        */
    CGVector operator/(double p)const;                      /**<vector div a real.        */
    CGVector operator-()const;                              /**<anti-direction vector.    */
    bool operator<(const CGPoint &p2)const;                 /**< point p1 < p2.           */
    bool operator>(const CGPoint &p2)const;                 /**< point p1 < p2.           */
    bool operator<=(const CGPoint &p2)const;                /**< point p1 < p2.           */
    bool operator>=(const CGPoint &p2)const;                /**< point p1 < p2.           */
};

double CGdist(const CGPoint &p);                                      /**< dist to (0. 0).          */
double CGdist(const CGPoint &p1, const CGPoint &p2);                  /**< dist from p1 to p2.      */
CGPoint CGunit(const CGVector &v);                                    /**< dest of unit vector.     */
double CGcross(const CGVector &v1, const CGVector &v2);               /**< v1,v2 croos product.     */
double CGdot(const CGVector &v1, const CGVector &v2);                 /**< v1,v2 dot product.       */
double CGarea(const CGPoint &a, const CGPoint &b, const CGPoint &c);  /**< triangle(a, b, c)'s area.*/
double CGangle(const CGVector &v1, const CGVector &v2);               /**< acos<v1, v2>.            */
CGPoint CGproject(const CGVector &v1, const CGVector &v2);            /**< project v1 on v2.        */

struct CGLine{
    CGPoint a, b;
    CGLine(){}
    CGLine(const CGPoint p1, CGPoint p2){
        a = p1, b = p2-p1;
    }
    CGLine(double a1, double b1, double c1);        /**< a1 x + b1 y + c1 >= 0 .            */
    double angle();                                 /**< line angle.                        */
    bool include(const CGPoint &p);                 /**< judge that p is on the line.       */
    bool onleft(const CGPoint &p);                  /**< judge that p is on the left(down). */
    bool onright(const CGPoint &p);                 /**< judge that p is on the right(up) . */

    bool operator==(const CGLine &ol);              /**< judge that self and ol is the same.*/
};

double CGdist(const CGPoint &p, const CGLine &l);   /**< the dist from point p to line l.   */
bool CGparallel(const CGLine &l1, const CGLine l2); /**< judge that l1 // l2.               */

HFCLOUD_END
