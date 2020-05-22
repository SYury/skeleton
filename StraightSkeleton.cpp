#include <algorithm>
#include <assert.h>

#include "Line.h"
#include "primitives.h"
#include "StraightSkeleton.h"


bool StraightSkeleton::Event::operator > (const StraightSkeleton::Event& ev)const{
    if(eq(time, ev.time)){
        return parents == ev.parents ? (int)type > (int)ev.type : parents > ev.parents;
    }
    else{
        return gt(time, ev.time);
    }
}

inline LAV::iterator prv(LAV * lav, LAV::iterator pos){
    return pos == lav->begin() ? --lav->end() : --pos;
}

inline LAV::iterator nxt(LAV * lav, LAV::iterator pos){
    return pos == --lav->end() ? lav->begin() : ++pos;
}

inline bool is_concave(Point l, Point curr, Point r){
    return lt(l.cross(curr, r), 0);
}

inline bool is_convex(Point l, Point curr, Point r){
    return !is_concave(l, curr, r);
}

void shift_lav(LAV * lav, Float time){
    //assuming LAV is shifted up to time' <= time
    std::vector<Point> new_pos;
    for(auto it = lav->begin(); it != lav->end(); it++){
        Vertex * v = *it;
        Vertex * l = *prv(lav, it);
        Vertex * r = *nxt(lav, it);
        Line b = bisector(l->curr_pos, v->curr_pos, r->curr_pos);
        auto vec = b.vec().normalize();
        vec *= (time - v->curr_time) / fabs(angle_sin(v->curr_pos + vec, v->curr_pos, r->curr_pos));
        new_pos.push_back(v->curr_pos + vec);
    }
    auto ptr = new_pos.begin();
    for(auto v: *lav){
        v->curr_pos = *ptr;
        v->curr_time = time;
        ++ptr;
    }
}

Vertex* StraightSkeleton::create_vertex(Point pt, Float time){
    Vertex * v = new Vertex{pt, vertices.size(), pt, time};
    dead_vertex.push_back(false);
    vertices.push_back(v);
    belongs_to.push_back(nullptr);
    lav_pos.push_back(LAV::iterator());
    return v;
}

void StraightSkeleton::add_edge_event(Vertex * v, Float time_passed){
    auto l = *prv(belongs_to[v->id], lav_pos[v->id]);
    auto r = *nxt(belongs_to[v->id], lav_pos[v->id]);
    auto pl = *prv(belongs_to[l->id], lav_pos[l->id]);
    auto pr = *nxt(belongs_to[r->id], lav_pos[r->id]);
    Line curr_bisector = bisector(l->curr_pos, v->curr_pos, r->curr_pos);
    Line left_bisector = bisector(pl->curr_pos, l->curr_pos, v->curr_pos);
    Line right_bisector = bisector(v->curr_pos, r->curr_pos, pr->curr_pos);
    bool par_left = !rays_intersect(left_bisector, curr_bisector);
    bool par_right = !rays_intersect(curr_bisector, right_bisector);
    if(!par_left || !par_right){
        Point event_pos;
        Vertex * other_vertex = nullptr;
        if(par_left){
            other_vertex = r;
            event_pos = inter_line_line(curr_bisector, right_bisector);
        }
        else if(par_right){
            other_vertex = l;
            event_pos = inter_line_line(curr_bisector, left_bisector);
        }
        else{
            Point R = inter_line_line(curr_bisector, right_bisector);
            Point L = inter_line_line(curr_bisector, left_bisector);
            if(lt(L.dot(v->curr_pos, R), 0)){
                other_vertex = l;
                event_pos = L;
            }
            else{
                other_vertex = r;
                event_pos = R;
            }
        }
        Float event_time = time_passed + std::min(dist_point_line(event_pos, Line(v->curr_pos, l->curr_pos)),
                                                  dist_point_line(event_pos, Line(v->curr_pos, r->curr_pos)));
        Event evt{event_time, event_pos, edge_event, std::make_tuple(v, other_vertex, nullptr)};
        q.push(evt);
    }
}

void StraightSkeleton::add_split_event(Vertex * v, Float time_passed){
    auto l = *prv(belongs_to[v->id], lav_pos[v->id]);
    auto r = *nxt(belongs_to[v->id], lav_pos[v->id]);
    if(is_convex(l->curr_pos, v->curr_pos, r->curr_pos))
        return;
    Line l_edge = Line(l->curr_pos, v->curr_pos);
    Line r_edge = Line(r->curr_pos, v->curr_pos);
    for(auto u = r; u != l; u = *nxt(belongs_to[u->id], lav_pos[u->id])){
        auto nu = *nxt(belongs_to[u->id], lav_pos[u->id]);
        Line opposite = Line(u->curr_pos, nu->curr_pos);
        Point event_pos = equidistant(l_edge, r_edge, opposite);
        if(isinf(event_pos.x) || isinf(event_pos.y) || isnan(event_pos.x) || isnan(event_pos.y))
            continue;
        Line b1 = bisector((*prv(belongs_to[u->id], lav_pos[u->id]))->curr_pos, u->curr_pos, nu->curr_pos);
        Line b2 = bisector(u->curr_pos, nu->curr_pos, (*nxt(belongs_to[nu->id], lav_pos[nu->id]))->curr_pos);
        if(in_triangle(event_pos, Line(u->curr_pos, nu->curr_pos), b1, b2)){
            Float event_time = time_passed + dist_point_line(event_pos, Line(u->curr_pos, nu->curr_pos));
            Event evt{event_time, event_pos, split_event, std::make_tuple(v, u, nu)};
            q.push(evt);
        }
        fflush(stdout);
    }
}

void StraightSkeleton::handle_edge_event(Event evt){
    Vertex *l, *r;
    std::tie(l, r, std::ignore) = evt.parents;
    if(dead_vertex[l->id] || dead_vertex[r->id])
        return;
    LAV *lav_l = belongs_to[l->id], *lav_r = belongs_to[r->id];
    if(lav_l != lav_r)
        return;
    shift_lav(belongs_to[l->id], evt.time);
    dead_vertex[l->id] = dead_vertex[r->id] = true;
    Vertex * I = create_vertex(evt.pos, evt.time);
    if(lav_l->size() == 3u){
        Vertex * v[3] = {*lav_l->begin(), *(++lav_l->begin()), *(++(++(lav_l->begin())))};
        for(size_t i = 0; i < 3; i++){
            dead_vertex[v[i]->id] = true;
            Edge * e = new Edge{v[i], I};
            result.push_back(e);
        }
        dead_vertex[I->id] = true;
        return;
    }
    Edge * el = new Edge{l, I};
    Edge * er = new Edge{r, I};
    result.push_back(el);
    result.push_back(er);
    auto l_iter = lav_pos[l->id];
    auto r_iter = lav_pos[r->id];
    if(std::next(l_iter) == r_iter || (std::next(l_iter) == lav_l->end() && r_iter == lav_l->begin())){
        lav_pos[I->id] = lav_l->insert(l_iter, I);
        lav_l->erase(l_iter);
        lav_l->erase(r_iter);
        belongs_to[I->id] = lav_l;
    }
    else{
        assert(std::next(r_iter) == l_iter || (std::next(r_iter) == lav_l->end() && l_iter == lav_l->begin()));
        lav_pos[I->id] = lav_l->insert(r_iter, I);
        lav_l->erase(l_iter);
        lav_l->erase(r_iter);
        belongs_to[I->id] = lav_l;
    }
    add_edge_event(I, evt.time);
    add_split_event(I, evt.time);
}

void StraightSkeleton::handle_split_event(Event evt){
    Vertex *v, *ul, *ur;
    std::tie(v, ul, ur) = evt.parents;
    if(dead_vertex[v->id] || dead_vertex[ul->id] || dead_vertex[ur->id])
        return;
    if(belongs_to[ul->id] != belongs_to[ur->id])
        return;
    dead_vertex[v->id] = true;
    shift_lav(belongs_to[v->id], evt.time);
    Vertex * v1 = create_vertex(evt.pos, evt.time);
    Vertex * v2 = create_vertex(evt.pos, evt.time);
    Edge * e = new Edge{v, v1};
    result.push_back(e);
    auto r = *nxt(belongs_to[v->id], lav_pos[v->id]);
    LAV * lav_l = new LAV();
    LAV * lav_r = new LAV();
    auto it = lav_pos[v->id];
    *it = v2;
    for(; *it != ur; it = nxt(belongs_to[v->id], it)){
        lav_r->push_back(*it);
        lav_pos[(*it)->id] = --lav_r->end();
        belongs_to[(*it)->id] = lav_r;
    }
    *lav_pos[v->id] = v1;
    for(; *it != r; it = nxt(belongs_to[v->id], it)){
        lav_l->push_back(*it);
        lav_pos[(*it)->id] = --lav_l->end();
        belongs_to[(*it)->id] = lav_l;
    }
    slav.erase((intptr_t)belongs_to[v->id]);
    delete belongs_to[v->id];
    belongs_to[v->id] = nullptr;
    slav.insert((intptr_t)lav_l);
    slav.insert((intptr_t)lav_r);
    add_edge_event(v1, evt.time);
    add_split_event(v1, evt.time);
    add_edge_event(v2, evt.time);
    add_split_event(v2, evt.time);
}

void StraightSkeleton::pre_initialize(){
    n = p.size();
    vertices.resize(n);
    dead_vertex.resize(n);
    for(size_t i = 0; i < n; i++){
        vertices[i] = new Vertex{p[i], i, p[i], 0.0};
        dead_vertex[i] = 0;
    }
    LAV * init_lav = new LAV();
    for(size_t i = 0; i < n; i++){
        init_lav->push_back(vertices[i]);
    }
    belongs_to = std::vector<LAV*>(n, init_lav);
    lav_pos.resize(n);
    auto tmp = init_lav->begin();
    for(size_t i = 0; i < n; i++){
        lav_pos[i] = tmp;
        ++tmp;
    }
    slav.insert((intptr_t)init_lav);
}

void StraightSkeleton::initialize_events(){
    for(size_t i = 0; i < n; i++){
        add_edge_event(vertices[i], 0);
        add_split_event(vertices[i], 0);
    }
}

void StraightSkeleton::main_loop(){
    while(!q.empty()){
        Event curr_event = q.top();
        q.pop();
        switch(curr_event.type){
            case edge_event:
                handle_edge_event(curr_event);
                break;
            case split_event:
                handle_split_event(curr_event);
                break;
            default:
                assert(false);
        }
    }
}

void StraightSkeleton::cleanup(){
    for(auto l: slav){
        delete (LAV*)l;
    }
}

Graph StraightSkeleton::build(){
    pre_initialize();
    initialize_events();
    main_loop();
    cleanup();
    return result;
}

