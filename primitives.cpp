#include <algorithm>
#include <assert.h>

#include "primitives.h"

Line bisector(Point l, Point mid, Point r){
    //the bisector points inside the polygon
    Point vl = l - mid;
    Point vr = r - mid;
    Line b(mid, vl.normalize() + vr.normalize() + mid);
    if(gt(b[0].cross(l, b[1]), 0))
        b[1] = b[0]*2 - b[1];
    return b;
}

bool are_parallel(Line l1, Line l2){
    return eq((l1[1] - l1[0]).cross(l2[1] - l2[0]), 0);
}

bool rays_intersect(Line l1, Line l2){
    //  \ l1
    // |
    // |
    // |
    // v
    //  / l2
    return gt(l1.vec().cross(l2.vec()), 0);
}

Point inter_line_line(Line l1, Line l2){
    if(are_parallel(l1, l2))
        return Point{INFINITY, INFINITY};
    Point u = l2[1] - l2[0];
    Point v = l1[1] - l1[0];
    Float s = u.cross(l2[0] - l1[0])/u.cross(v);
    return l1[0] + v * s;
}

Float dist_point_line(Point p, Line l){
    return fabs(p.cross(l[0], l[1]))/(l[0] - l[1]).norm();
}

Point equidistant(Line l1, Line l2, Line l3){
    // \/
    //
    // -->
    if(are_parallel(l1, l3)){
        std::swap(l1, l2);
        std::swap(l3[0], l3[1]);
    }
    Point i2 = inter_line_line(l1, l2);
    Point i3 = inter_line_line(l1, l3);
    Line b2 = bisector(i2 + l2.vec(), i2, i2 + l1.vec());
    Line b3 = bisector(i3 - l1.vec(), i3, i3 + l3.vec());
    return inter_line_line(b2, b3);
}

bool in_triangle(Point p, Line base, Line l1, Line l2){
    //this function checks if p belongs to the pseudo-triangle
    //formed by base and l1, l2 that intersect it
    //the direction of triangle edges is l1[0]->l1[1], l2[0]->l2[1]
    //it is guaranteed that l1[0] == base[0], l2[0] == base[1]
    //base[0]->base[1]->l2[1] must be ccw
    return le(base[0].cross(p, base[1]), 0) && \
           le(l2[0].cross(p, l2[1]), 0) && \
           le(l1[0].cross(l1[1], p), 0);
}

Float angle_sin(Point l, Point mid, Point r){
    return std::clamp(mid.cross(l, r)/((l - mid).norm() * (r - mid).norm()), (Float)-1, (Float)1);
}
