//
//  base_ant.cpp
//  models
//
//  Created by Aditi Lath on 2017-07-17.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#include "base_ant.hpp"


using namespace std;

# define INF 0x3f3f3f3f

base_ant::base_ant() { }

base_ant::~base_ant() { }


iPair base_ant::cost_node(int time) {
    if (counter <= 0) {
        current = ordered_path.front();
        ordered_path.pop();
        
        if (ordered_path.front()->get_id() == dest) {
            t_node* prev = current;
            current = ordered_path.front();
            ordered_path.pop();
            counter = 0;
            return make_pair(prev->get_id(), current->get_id());
        }
        
        if (current != ordered_path.front()) {
            int n_nodeid = ordered_path.front()->get_id();
            for (int i = 0; i < current->edge_number(); i++) {
                t_edge* e = (*current)[i];
                if (n_nodeid == e->get_dest()->get_id()) {
                    counter = e->get_time_to_cross() - 1;
                }
            }
        }
        
        return make_pair(current->get_id(), ordered_path.front()->get_id());
    }
    counter--;
    return make_pair(INF, INF);
}

bool base_ant::has_reached_destination() {
    if (counter > 0) {
        return false;
    }
    return current->get_id() == dest;
}

queue<t_node*> base_ant::get_ordered_path() {
    return ordered_path;
}

void base_ant::init_cost() {
    current = ordered_path.front();
}
