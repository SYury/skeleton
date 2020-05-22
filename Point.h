#ifndef POINT_H_
#define POINT_H_

#include "float_math.h"

struct Point{
    Float x, y;
    Point(){}
    Point(Float _x, Float _y):x(_x), y(_y){}
    Point operator + (const Point & p)const;
    Point operator - (const Point & p)const;
    Point operator * (const Float & f)const;
    Point operator / (const Float & f)const;
    Point normalize ()const;
    Point& operator += (const Point & p);
    Point& operator -= (const Point & p);
    Point& operator *= (const Float & f);
    Point& operator /= (const Float & f);
    Float cross(const Point & p)const;
    Float cross(const Point & p, const Point & q)const;
    Float dot(const Point & p)const;
    Float dot(const Point & p, const Point & q)const;
    Float norm()const;
};

#endif // POINT_H_
