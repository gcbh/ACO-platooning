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
    if (ordered_path.size() > 0) {
        current = ordered_path.front();
        if (counter <= 0) {
            ordered_path.pop();
            t_node* next = ordered_path.front();
            for (int i = 0; i < current->edge_number(); i++) {
                t_edge* e = (*current)[i];
                if (e->get_dest() == next) {
                    counter = (*current)[i]->get_time_to_cross() - 1;
                    return make_pair(current, e);
                }
            }
        }
    }
    --counter;
    return make_pair(nullptr, nullptr);
}

bool base_ant::has_reached_destination() {
    return (current->get_id() == dest && counter <= 0);
}

bool base_ant::has_concluded() {
    return has_reached_destination();
}

queue<t_node*> base_ant::get_ordered_path() {
    return ordered_path;
}
