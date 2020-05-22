#ifndef FLOAT_MATH_H_
#define FLOAT_MATH_H_

#include <math.h>

using Float = long double;

constexpr Float eps = 1e-9;

inline bool eq (Float a, Float b){
    return fabs(a - b) < eps;
}

inline bool lt (Float a, Float b){
    return a + eps < b;
}

inline bool gt (Float a, Float b){
    return a - eps > b;
}

inline bool le (Float a, Float b){
    return !gt(a, b);
}

inline bool ge (Float a, Float b){
    return !lt(a, b);
}

inline Float safe_sqrt(Float x){
    return x < 0 ? 0 : sqrt(x);
}

#endif // FLOAT_MATH_H
