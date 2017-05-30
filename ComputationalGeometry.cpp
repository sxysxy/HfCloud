#include "ComputationalGeometry.h"

HFCLOUD_BEGIN

double CGPoint::length(){
    return sqrt(x*x+y*y);
}
CGPoint& CGPoint::unit(){
    double d = length();
    if(CGdcmp(d) > 0){
        x /= d;
        y /= d;
    }
    return *this;
}
CGPoint& CGPoint::rotate(double angle){
    double tx = x;
    x = cos(angle)*x-sin(angle)*y;
    y = sin(angle)*tx+cos(angle)*y;
    return *this;
}
CGPoint& CGPoint::setlength(double len){
    double olen = length();
    HFASSERT(olen, "The length of a vector should not be zero when reset its length(or we do not know its direction)")
    if(CGdcmp(olen) > 0)
        x *= len/olen, y *= len/olen;
    return *this;
}
double CGPoint::angle(){
    double ang = atan2(y, x);
    if(CGdcmp(ang) < 0)ang += CG_2PI;
    return ang;
}
bool CGPoint::operator==(const CGPoint &p2)const{
    return CGdcmp(x-p2.x) == 0 && CGdcmp(y-p2.y) == 0;
}
bool CGPoint::operator!=(const CGPoint &p2)const{
    return !(*this == p2);
}
CGVector CGPoint::operator+(const CGVector &v2)const{
    return CGVector(x+v2.x, y+v2.y);
}
CGVector CGPoint::operator-(const CGPoint &p2)const{
    return CGVector(x-p2.x, y-p2.y);
}
CGVector CGPoint::operator*(double p)const{
    return CGVector(x*p, y*p);
}
CGVector CGPoint::operator/(double p)const{
    return CGVector(x/p, y/p);
}
CGVector CGPoint::operator-()const{
    return CGVector(-x, -y);
}
bool CGPoint::operator<(const CGPoint &p2)const{
    return (*this != p2)? x<p2.x : y<p2.y;
}
bool CGPoint::operator>(const CGPoint &p2)const{
    return *this != p2? x>p2.x : y>p2.y;
}
bool CGPoint::operator<=(const CGPoint &p2)const{
    return *this < p2 || *this == p2;
}
bool CGPoint::operator>=(const CGPoint &p2)const{
    return *this > p2 || *this == p2;
}

//-----------------------

double CGdist(const CGPoint &p){
    return sqrt(p.x*p.x+p.y+p.y);
}
double CGdist(const CGPoint &p1, const CGPoint &p2){
    return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}
CGPoint CGunit(const CGVector &v){
    double d = sqrt(v.x*v.x+v.y*v.y);
    if(CGdcmp(d) > 0)return v/d;
    else return v;
}
double CGcross(const CGVector &v1, const CGVector &v2){
    return v1.x*v2.y-v2.x*v1.y;
}
double CGdot(const CGVector &v1, const CGVector &v2){
    return v1.x*v2.x+v1.y*v2.y;
}
double CGarea(const CGPoint &a, const CGPoint &b, const CGPoint &c){
    return fabs(CGcross(b-a, c-a));
}
double CGangle(const CGVector &v1, const CGVector &v2){
    return acos(CGdot(v1, v2)/CGdist(v1)/CGdist(v2));
}
CGPoint CGproject(const CGVector &v1, const CGVector &v2){
    double dv2 = (v2.x*v2.x+v2.y*v2.y);
    if(CGdcmp(dv2) > 0)return v2*(CGdot(v1, v2)/dv2);
    else return CGPoint(0, 0);
}

//---------------

CGLine::CGLine(double a1, double b1, double c1){
    if(CGdcmp(b1-a1)){
        a = CGPoint(0, -c1/b1);
        if(CGdcmp(b1) > 0)b = CGPoint(1, -a1/b1);
        else b = CGPoint(-1, a1/b1);
    }else if(CGdcmp(a1)){
        a = CGPoint(-c1/a1, 0);
        if(CGdcmp(a1) > 0)
            b = CGPoint(b1/a1, -1);
        else b = CGPoint(-b1/a1, 1);
    }
}
double CGLine::angle(){
    double ang = atan2(b.y, b.x);
    if(ang < 0)ang += CG_PI;
    if(!CGdcmp(CG_PI-ang))ang = 0;
    return ang;
}
bool CGLine::include(const CGPoint &p){
    return !CGdcmp(CGcross(a-p, b));
}
bool CGLine::onleft(const CGPoint &p){
    return CGdcmp(CGcross(b, p-a)) > 0;
}
bool CGLine::onright(const CGPoint &p){
    return CGdcmp(CGcross(b, p-a)) < 0;
}
bool CGLine::operator==(const CGLine &ol){
    return include(ol.a) && include(ol.b);
}

//-------------------------------------------------
double CGdist(const CGPoint &p, const CGLine &l){
    return fabs(CGcross(l.b, p-l.a))/CGdist(l.b);
}
bool CGparallel(const CGLine &l1, const CGLine l2){
    return CGunit(l1.b) == CGunit(l2.b) || CGunit(l1.b) == -CGunit(l2.b);
}

HFCLOUD_END
