#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>

#include "Point.h"

struct Vertex{
    Point pos;
    size_t id;
    Point curr_pos;
    Float curr_time;
};

struct Edge{
    Vertex *v, *u;
};

using Graph = std::vector<Edge*>;

#endif // GRAPH_H_
