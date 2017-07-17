//
//  ant.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#include "ant.hpp"

using namespace std;

ant::ant(t_node* first, Dijkstra* i_d_map, int i_dest, float i_alpha, float i_beta, float i_delta, float i_phi, Randoms* i_r) {
    d_map = i_d_map;
    probability = i_r;
    dest = i_dest;
    counter = 0;
    current = first;
    ALPHA = i_alpha;
    BETA = i_beta;
    DELTA = i_delta;
    PHI = i_phi;
    v_route = false;
}

ant::~ant() {   }

void ant::next_node(int time) {
    if (counter <= 0) {
        double total = 0;
        double total_prob = 0;
        float best_wait = 0;
        int past_travelled = 0;
        for (int i = 0; i < current->edge_number(); i++) {
            
            t_edge* e = (*current)[i];
            
            int n_nodeid = e->get_dest()->get_id();
            int e_id = e->get_id();
            
            if (past_nodes.find(n_nodeid) == past_nodes.end()) {
                pheromone p = e->get_pheromone(time);
                
                double test = calculate_heuristic(n_nodeid, e->get_distance(), p.current);
                if (!test) {
                    string a = "";
                }
                total += test;
                
                // determines largest future pheromone
                if (p.future > best_wait) {
                    best_wait = p.future;
                }
            } else {
                past_travelled++;
            }
        }
        
        if (past_travelled == current->edge_number()) {
            v_route = true;
            return;
        }
        
        total += calculate_heuristic(current->get_id(), 0, best_wait);
        
        // random value between 0 and 1
        double prob = probability->Uniforme();
        
        for (int i = 0; i < current->edge_number(); i++) {
            t_edge* e = (*current)[i];
            
            int n_nodeid = e->get_dest()->get_id();
            int e_id = e->get_id();
            
            if (past_nodes.find(n_nodeid) == past_nodes.end()) {
                pheromone p = e->get_pheromone(time);
                // sums to find position of random variable between 0 and 1
                total_prob += calculate_heuristic(n_nodeid, e->get_distance(), p.current);
            
                // if in range of current edge
                if (total_prob/total >= prob) {
                    e->update_pheromone(time, 1.0f);
                    // ensure node travelling from cannot be reached again
                    past_nodes.insert(n_nodeid);
                    // current node included in up-to-date path
                    ordered_path.push(current);
                    // update 'current'
                    current = e->get_dest();
                    // update 'counter' for timing
                    counter = e->get_time_to_cross() - 1;
                    
                    if (has_reached_destination()) {
                        ordered_path.push(current);
                    }
                    
                    return;
                }
            }
        }
        // portion of spectrum for 'WAIT' option
        total_prob += calculate_heuristic(current->get_id(), 0, best_wait);
        
        if (total_prob / total >= prob) {
            ordered_path.push(current);
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

double ant::calculate_heuristic(int node_id, int e_dist, float ph) {
    float p, d;
    
    int distance = d_map->get_edge_weight(node_id, dest);
    
    d = 1.0f / (e_dist + distance);
    
    if (!ph) {
        return PHI*d;
    }
    
    return pow(ph, ALPHA) * pow(d, BETA);
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
