//
//  ant.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#include "ant.hpp"

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
    // need to incorporate dijkstra data into decision
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
        
        // generate weighted PHI (explore) value
        float prob_phi = total * PHI;
        
        total += prob_phi;
        
        // proportion of total for any untravelled path
        prob_phi /= untravelled;
        
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
                    // ensure node travelling from cannot be reached again
                    past_nodes.insert(ordered_path.back());
                    // current node included in up-to-date path
                    ordered_path.push_back(current->get_id());
                    // update 'current'
                    current = e->get_dest();
                    // update 'counter' for timing
                    counter = e->get_time_to_cross();
                    return;
                }
            }
        }
        // portion of spectrum for 'WAIT' option
        total_prob += calculate_heuristic(current->get_id(), best_wait);
        
        if (total_prob >= prob) {
            ordered_path.push_back(current->get_id());
        } else {
            string e = "Issue with pathfinding. Should not get past final probability. Total: " + to_string(total_prob) + " Current: " + to_string(prob);
            throw e;
        }
        return;
    }
    counter--;
}

double ant::calculate_heuristic(int node_id, float ph) {
    float p, d;
    
    p = tanh(ph);
    d = 1 / d_map->get_edge_weight(node_id, dest);
    
    return ALPHA*p + BETA*d;
}

iPair ant::cost_node(int time) {
    if (counter <= 0) {
        
    }
}


bool ant::has_reached_destination() {
    return current->get_id() == dest;
}

vector<int> ant::get_ordered_path() {
    return ordered_path;
}
