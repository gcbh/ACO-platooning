//
//  ant.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#include "ant.hpp"

using namespace std;

ant::ant(t_node* first, int i_dest, float d, heuristic_selector* sel) {
    dest = i_dest;
    counter = 0;
    DELTA = d;
    current = first;
    v_route = false;
    selector = sel;
    ordered_path.push(current);
    // past_edges.insert(current->get_id());
}

ant::~ant() {   }

path ant::next_node(int time) {
    if (counter <= 0 && !has_concluded()) {
        
        list<t_edge*> es = avail_edges();
        
        if (es.size() == 0) {
            v_route = true;
            return make_pair(nullptr, nullptr);
        }
        
        t_edge* e = selector->selected_edge(es, current->get_id(), dest, time);
        
        if (!e) {
            ordered_path.push(current);
            return make_pair(current, nullptr);
        }
        
        t_node* next = e->get_dest();
        
        // current node included in up-to-date path
        ordered_path.push(next);
        
        e->update_pheromone(time, DELTA);
        // ensure node travelling from cannot be reached again
        past_edges.insert(e->get_id());
        path p = make_pair(current, e);
        
        // update 'current'
        current = next;
        // update 'counter' for timing
        counter = e->get_time_to_cross() - 1;
        
        return p;
    }
    counter--;
    return make_pair(nullptr, nullptr);
}

list<t_edge*> ant::avail_edges() {
    list<t_edge*> edges;
    for (int i = 0; i < current->edge_number(); i++) {
        t_edge* e = (*current)[i];
        if (past_edges.find(e->get_id()) == past_edges.end()) {
            edges.push_back(e);
        }
    }
    return edges;
}

bool ant::has_concluded() {
    return v_route ^ has_reached_destination();
}

bool ant::void_route() {
    return v_route;
}
