#include <stdio.h>
#include <vector>

#include "dump.h"
#include "Point.h"
#include "StraightSkeleton.h"

int main(int argc, char ** argv){
    freopen(argv[1], "r", stdin);
    int n;
    scanf("%d", &n);
    std::vector<Point> p(n);
    for(int i = 0; i < n; i++){
        scanf("%Lf%Lf", &p[i].x, &p[i].y);
    }
    auto builder = StraightSkeleton(p.begin(), p.end());
    dump(p, builder.build(), argv[2]);
    return 0;
}
