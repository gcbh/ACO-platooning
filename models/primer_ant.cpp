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
    counter = 0;
    current = first;
    route = route_path;
    dest = stoi(route.back());
}

path primer_ant::next_node(int time) {
    if (counter <= 0) {
        
        // Remove the first element from the vector
        route.erase(route.begin());

        int next_node = stoi(route.front());
        t_edge* e = current->get_edge(next_node);

        e->update_pheromone(time, 1.0f);

        // current node included in up-to-date path
        ordered_path.push(current);
        // update 'current'
        current = e->get_dest();
        // update 'counter' for timing
        counter = e->get_time_to_cross() - 1;
                
        if (has_reached_destination()) {
            ordered_path.push(current);
        }

        return make_pair(nullptr, nullptr);

    }
        
    counter--;
    
    if (has_reached_destination()) {
        ordered_path.push(current);
    }
    return make_pair(nullptr, nullptr);
}
