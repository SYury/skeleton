#include "Line.h"

Point& Line::operator [] (const int & i){
    return p[i];
}

Point Line::vec()const{
    return p[1] - p[0];
}
