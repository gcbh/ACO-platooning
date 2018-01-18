//
//  heuristic_selector.cpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-01-12.
//

#include "heuristic_selector.hpp"

heuristic_selector::heuristic_selector(float a, float b, float p, long seed, Dijkstra* map): probability(seed) {
    ALPHA = a;
    BETA = b;
    PHI = p;
    d_map = map;
}

t_edge* heuristic_selector::selected_edge(list<t_edge*> edges, int current_id, int dest_id, int time) {
    double total = 0, total_prob = 0, best_wait = 0;

    for (list<t_edge*>::iterator it = edges.begin(); it != edges.end(); ++it) {
        t_edge* e = (*it);
        int n_nodeid = e->get_dest()->get_id();
        int e_id = e->get_id();
        
        pheromone p = e->get_pheromone(time);
        
        total += calculate_heuristic(n_nodeid, dest_id, e->get_distance(), p.current);
        
        if (p.future > best_wait) {
            best_wait = p.future;
        }
    }
    
    total += calculate_heuristic(current_id, dest_id, 0, best_wait);
    
    double prob = probability.Uniforme();
    
    for (list<t_edge*>::iterator it = edges.begin(); it != edges.end(); ++it) {
        t_edge* e = (*it);
        int n_nodeid = e->get_dest()->get_id();
        int e_id = e->get_id();
        
        pheromone p = e->get_pheromone(time);
        
        total_prob += calculate_heuristic(n_nodeid, dest_id, e->get_distance(), p.current);
        
        if (total_prob/total >= prob) {
            return e;
        }
    }
    
    return nullptr;
}

double heuristic_selector::calculate_heuristic(int node_id, int dest_id, int e_dist, float ph) {
    float p, d;
    
    int distance = d_map->get_edge_weight(node_id, dest_id);
    
    d = 1.0f / (e_dist + distance) + 1.0f;
    
    if (!ph || floor(ph) == 0.0f) {
        return PHI*d;
    }
    
    return pow(ph, ALPHA) * pow(d, BETA);
}
