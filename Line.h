#ifndef LINE_H_
#define LINE_H_

#include "Point.h"

struct Line{
    Point p[2];
    Line(){}
    Line(Point a, Point b):p{a, b}{}
	Point& operator [](const int & i);
    Point vec()const;
};

#endif // LINE_H_
