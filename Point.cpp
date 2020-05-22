#include "Point.h"

Point Point::operator + (const Point & p)const{
    return Point(x + p.x, y + p.y);
}

Point Point::operator - (const Point & p)const{
    return Point(x - p.x, y - p.y);
}

Point Point::operator * (const Float & f)const{
    return Point(x * f, y * f);
}

Point Point::operator / (const Float & f)const{
    return Point(x / f, y / f);
}

Point Point::normalize ()const{
    return *this / norm();
}

Point& Point::operator += (const Point & p){
    x += p.x; y += p.y;
    return *this;
}

Point& Point::operator -= (const Point & p){
    x -= p.x; y -= p.y;
    return *this;
}

Point& Point::operator *= (const Float & f){
    x *= f; y *= f;
    return *this;
}

Point& Point::operator /= (const Float & f){
    x /= f; y /= f;
    return *this;
}

Float Point::cross (const Point & p)const{
    return x * p.y - y * p.x;
}

Float Point::cross (const Point & p, const Point & q)const{
    return (p - *this).cross(q - *this);
}

Float Point::dot (const Point & p)const{
    return x * p.x + y * p.y;
}

Float Point::dot (const Point & p, const Point & q)const{
    return (p - *this).dot(q - *this);
}

Float Point::norm ()const{
    return safe_sqrt(this->dot(*this));
}
