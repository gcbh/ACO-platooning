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

path base_ant::next_node(int time) {
    return make_pair(nullptr, nullptr);
}

path base_ant::replay_route() {
    --counter;
    if ((ordered_path.size() > 1 && !in_transit())) {
        current = ordered_path.front();
        
        ordered_path.pop();
        
        t_node* next = ordered_path.front();
        t_edge* e = current->get_edge(next->get_id());
        
        if (!e) return make_pair(current, nullptr);
        
        counter = e->get_time_to_cross();
        
        return make_pair(current, e);
    }
    return make_pair(nullptr, nullptr);
}

bool base_ant::has_reached_destination() {
    return (current->get_id() == dest && counter <= 0);
}

bool base_ant::has_concluded() {
    return has_reached_destination();
}

bool base_ant::in_transit() {
    return counter > 0;
}

queue<t_node*> base_ant::get_ordered_path() {
    return ordered_path;
}
