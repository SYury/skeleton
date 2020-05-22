#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include "Line.h"
#include "Point.h"

Line bisector(Point, Point, Point);
bool are_parallel(Line, Line);
bool rays_intersect(Line, Line);
Point inter_line_line(Line, Line);
Float dist_point_line(Point, Line);
Point equidistant(Line, Line, Line);
bool in_triangle(Point, Line, Line, Line);
Float angle_sin(Point, Point, Point);

#endif // PRIMITIVES_H_
