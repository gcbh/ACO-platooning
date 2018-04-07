//
//  ant.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#include "ant.hpp"

using namespace std;

ant::ant(t_node* first, int i_dest, float i_alpha, float i_beta, Randoms* i_r) {
    probability = i_r;
    dest = i_dest;
    counter = 0;
    current = first;
    alpha = i_alpha;
    beta = i_beta;
}

ant::~ant() {   }

void ant::next_node(int time) {
    // need to incorporate dijkstra data into decision
    if (counter <= 0) {
        double total = 0;
        double total_prob = 0;
        int best_wait = 0;
        for (int i = 0; i < current->edge_number(); i++) {
            t_edge* e = (*current)[i];
            if (past_nodes.find(e->get_dest()->get_id()) == past_nodes.end()) {
                phermone p = e->get_phermone(time);
                
                // TODO: include Dijkstra
                total += (double)p.current * alpha;
                
                // determines largest future pheromone
                if (p.future > best_wait) {
                    best_wait = p.future;
                }
            }
        }
        
        // 'alpha' is the weighting of staying, may need new weight
        total += (double)best_wait * alpha;
        
        double prob = probability->Uniforme();
        for (int i = 0; i < current->edge_number(); i++) {
            t_edge* e = (*current)[i];
            if (past_nodes.find(e->get_dest()->get_id()) == past_nodes.end()) {
                phermone p = e->get_phermone(time);
                
                // sums to find position of random variable between 0 and 1
                total_prob += alpha * p.current / total;
                
                // if in range of current edge
                if (total_prob >= prob) {
                    int d = e->get_dest()->get_id();
                    // ensure node travelling from cannot be reached again
                    past_nodes.insert(ordered_path.back());
                    // current node included in up-to-date path
                    ordered_path.push_back(current->get_id());
                    
                    // update 'current'
                    current = e->get_dest();
                    // update 'counter' for timing
                    counter = e->get_time_to_cross() - 1;
                    return;
                }
            }
        }
        ordered_path.push_back(current->get_id());
        return;
    }
    counter--;
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
