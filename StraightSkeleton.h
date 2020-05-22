#ifndef STRAIGHTSKELETON_H_
#define STRAIGHTSKELETON_H_

#include <algorithm>
#include <list>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

#include "Graph.h"
#include "Point.h"

using LAV = std::list<Vertex*>;

class StraightSkeleton{
protected:

    enum evt_type{
        edge_event,
        split_event
    };

    struct Event{
        Float time;
        Point pos;
        evt_type type;
        std::tuple<Vertex*, Vertex*, Vertex*> parents;

        bool operator > (const Event& ev)const;
    };

public:
    Graph build();

    template<class RandomAccessIterator>
    StraightSkeleton(RandomAccessIterator from, RandomAccessIterator to){
        p = std::vector<Point>(to - from);
        std::copy(from, to, p.begin());
    }

protected:
    Vertex* create_vertex(Point, Float);

    void add_edge_event(Vertex*, Float);
    void add_split_event(Vertex*, Float);

    void handle_edge_event(Event);
    void handle_split_event(Event);

    void pre_initialize();
    void initialize_events();
    void main_loop();
    void cleanup();

    Graph result;
    std::set<intptr_t> slav;
    std::vector<LAV*> belongs_to;
    std::vector<LAV::iterator> lav_pos;
    std::vector<char> dead_vertex;
    size_t n;
    std::vector<Point> p;
    std::priority_queue<Event, std::vector<Event>, std::greater<Event> > q;
    std::vector<Vertex*> vertices;
};

#endif // STRAIGHTSKELETON_H_
