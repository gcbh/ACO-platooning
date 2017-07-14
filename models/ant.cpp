//
//  ant.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#include "ant.hpp"

# define INF 0x3f3f3f3f

using namespace std;

ant::ant(t_node* first, Dijkstra* i_d_map, int i_dest, float i_alpha, float i_beta, float i_phi, Randoms* i_r) {
    d_map = i_d_map;
    probability = i_r;
    dest = i_dest;
    counter = 0;
    current = first;
    ALPHA = i_alpha;
    BETA = i_beta;
    PHI = i_phi;
}

ant::~ant() {   }

void ant::next_node(int time) {
    if (counter <= 0) {
        double total = 0;
        double total_prob = 0;
        float best_wait = 0;
        int untravelled = 0;
        for (int i = 0; i < current->edge_number(); i++) {
            
            t_edge* e = (*current)[i];
            
            int n_nodeid = e->get_dest()->get_id();
            if (past_nodes.find(n_nodeid) == past_nodes.end()) {
                if (e->pheromone_exists(time)) {
                    pheromone p = e->get_pheromone(time);
                    
                    total += calculate_heuristic(n_nodeid, p.current);
                    
                    // determines largest future pheromone
                    if (p.future > best_wait) {
                        best_wait = p.future;
                    }
                } else {
                    untravelled++;
                }
            }
        }
        
        total += calculate_heuristic(current->get_id(), best_wait);
        
        // proportion of total for any untravelled path
        // generate weighted PHI (explore) value
        float prob_phi = total * PHI;
        
        if (untravelled != 0) {
            total += prob_phi;
            prob_phi /= untravelled;
        }
        
        // random value between 0 and 1
        double prob = probability->Uniforme();
        
        for (int i = 0; i < current->edge_number(); i++) {
            t_edge* e = (*current)[i];
            int n_nodeid = e->get_dest()->get_id();
            if (past_nodes.find(n_nodeid) == past_nodes.end()) {
                
                if (e->pheromone_exists(time)) {
                    pheromone p = e->get_pheromone(time);
                    // sums to find position of random variable between 0 and 1
                    total_prob += calculate_heuristic(n_nodeid, p.current);
                } else {
                    total_prob += prob_phi;
                }
                
                // if in range of current edge
                if (total_prob >= prob) {
                    e->update_pheromone(time, 1.0f);
                    // ensure node travelling from cannot be reached again
                    past_nodes.insert(current->get_id());
                    // current node included in up-to-date path
                    ordered_path.push_back(current);
                    // update 'current'
                    current = e->get_dest();
                    // update 'counter' for timing
                    counter = e->get_time_to_cross() - 1;
                    return;
                }
            }
        }
        // portion of spectrum for 'WAIT' option
        total_prob += calculate_heuristic(current->get_id(), best_wait);
        
        if (total_prob >= prob) {
            ordered_path.push_back(current);
        } else {
            string e = "Issue with pathfinding. Should not get past final probability. Total: "
                + to_string(total_prob)
                + " Current: "
                + to_string(prob);
            throw e;
        }
        return;
    }
    counter--;
    
    if (has_reached_destination()) {
        ordered_path.push_back(current);
    }
}

double ant::calculate_heuristic(int node_id, float ph) {
    float p, d;
    
    p = tanh(ph);
    d = 1 / d_map->get_edge_weight(node_id, dest);
    
    return ALPHA*p + BETA*d;
}

iPair ant::cost_node(int time) {
    if (counter <= 0) {
        current = ordered_path.front();
        ordered_path.pop_front();
        if (current != ordered_path.front()) {
            int n_nodeid = ordered_path.front()->get_id();
            for (int i = 0; i < current->edge_number(); i++) {
                t_edge* e = (*current)[i];
                if (n_nodeid == e->get_dest()->get_id()) {
                    counter = e->get_time_to_cross();
                }
            }
        }
        return make_pair(current->get_id(), ordered_path.front()->get_id());
    }
    counter--;
    return make_pair(INF, INF);
}


bool ant::has_reached_destination() {
    if (counter > 0) {
        return false;
    }
    return current->get_id() == dest;
}
