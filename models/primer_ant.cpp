//
//  primer_ant.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#include "primer_ant.hpp"

using namespace std;


primer_ant:: primer_ant(t_node *first, vector<string> route_path) {
    current = first;
    route = route_path;
}

void primer_ant:: set_ant_path() {

    int tick = 0;

    // get time to cross every edge and update phermone
    for (int i = 1; i < route.size(); i++) {

        for (int j = 0; j < current->edge_number(); j++) {
            t_edge *cur_edge = (*current)[j];
            t_node *next_n = cur_edge->get_dest();
            if (next_n->get_id() == stoi(route[i])) {
                // TODO: set value to increase to
                cur_edge->update_pheromone(tick, 1.0f);
                int time_to_cross = cur_edge->get_time_to_cross();
                tick = tick + time_to_cross;
                current = next_n;
                break;
            }
        }
    }
}
