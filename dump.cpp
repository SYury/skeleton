#include <stdio.h>

#include "dump.h"

void dump(std::vector<Point> poly, Graph g, const char* filename){
    auto out = fopen(filename, "w");
    fprintf(out, "%d\n", (int)poly.size());
    for(size_t i = 0; i < poly.size(); i++){
        size_t j = i + 1 < poly.size() ? i + 1 : 0;
        fprintf(out, "%Lf %Lf %Lf %Lf\n", poly[i].x, poly[i].y, poly[j].x, poly[j].y);
    }
    fprintf(out, "%d\n", (int)g.size());
    for(auto e: g){
        fprintf(out, "%Lf %Lf %Lf %Lf\n", e->v->pos.x, e->v->pos.y, e->u->pos.x, e->u->pos.y);
    }
    fclose(out);
}
