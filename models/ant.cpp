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
}

ant::~ant() {   }

void ant::next_node(int time) {
    if (counter <= 0) {
        
        int past_travelled = 0;
        list<t_edge*> es = avail_edges();
        if (es.size() == 0) {
            v_route = true;
            return;
        }
        t_edge* e = selector->selected_edge(es, current->get_id(), dest, time);
        
        // current node included in up-to-date path
        ordered_path.push(current);
        
        if (!e) return;
        
        e->update_pheromone(time, 1.0f);
        // ensure node travelling from cannot be reached again
        past_nodes.insert(current->get_id());
        // update 'current'
        current = e->get_dest();
        // update 'counter' for timing
        counter = e->get_time_to_cross() - 1;
        
        if (has_reached_destination()) {
            ordered_path.push(current);
        }
        
        return;
    }
    counter--;
    
    if (has_reached_destination()) {
        ordered_path.push(current);
    }
}

iPair ant::cost_node(int time) {
    return base_ant::cost_node(time);
}

void ant::roll_back(int time, float magnitude) {
    if (counter <= 0 && ordered_path.size() > 1) {
        current = ordered_path.front();
        ordered_path.pop();
        for (int i = 0; i < current->edge_number(); i++) {
            if ((*current)[i]->get_dest()->get_id() == ordered_path.front()->get_id()) {
                counter = (*current)[i]->get_time_to_cross() - 1;
                (*current)[i]->update_pheromone(time, -1.0f * magnitude);
            }
        }
    }
    counter--;
}

list<t_edge*> ant::avail_edges() {
    list<t_edge*> edges;
    for (int i = 0; i < current->edge_number(); i++) {
        t_edge* e = (*current)[i];
        if (past_nodes.find(e->get_dest()->get_id()) == past_nodes.end()) {
            edges.push_back((*current)[i]);
        }
    }
    return edges;
}

queue<t_node*> ant::get_ordered_path() {
    return base_ant::get_ordered_path();
}

void ant::init_cost() {
    base_ant::init_cost();
}

bool ant::has_reached_destination() {
    return base_ant::has_reached_destination();
}

bool ant::void_route() {
    return v_route;
}
