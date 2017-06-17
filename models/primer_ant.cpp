//
//  primer_ant.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#include "primer_ant.hpp"

using namespace std;

primer_ant::primer_ant(std::queue<int> i_nodes, t_node *first) {
    ordered_path = i_nodes;
    current = first;
}

void primer_ant::next_node(int time) {
    int next = ordered_path->poll();
    for (int i = 0; i < current->edge_number(); i++) {
        t_edge cur_edge = current[i];
        if (cur_edge->dest->id == next) {
            // TODO: set value to increase to
            cur_edge.phermones_at[time] += 1.0f;
            current = cur_edge->dest;
            break;
        }
    }
}
