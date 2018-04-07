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
    double max_pheromone = 1.0;
    
    // get maximum pheromone of all the edges available including "wait" pheromone
    for (list<t_edge*>::iterator it = edges.begin(); it != edges.end(); ++it) {
        t_edge* e = (*it);
        pheromone p = { .current = 0.0f, .future = 0.0f };
        if (e->pheromone_exists(time)) {
            p = e->get_pheromone(time);
        }
        
        if (p.current > max_pheromone) {
            max_pheromone = p.current;
        }
        
        if (p.future > max_pheromone) {
            max_pheromone = p.future;
        }
    }
    
    // calculate total of all heuristics for each edge
    for (list<t_edge*>::iterator it = edges.begin(); it != edges.end(); ++it) {
        t_edge* e = (*it);
        int n_nodeid = e->get_dest()->get_id();
        int e_id = e->get_id();
        
        pheromone p = e->get_pheromone(time);
        
        total += calculate_heuristic(n_nodeid, dest_id, e->get_distance(), p.current, max_pheromone);
        
        if (p.future > best_wait) {
            best_wait = p.future;
        }
    }
    
    // add "wait" heuristic to total
    total += calculate_heuristic(current_id, dest_id, 0, best_wait, max_pheromone);
    
    double prob = probability.Uniforme();
    
    // pick edge to traverse
    for (list<t_edge*>::iterator it = edges.begin(); it != edges.end(); ++it) {
        t_edge* e = (*it);
        int n_nodeid = e->get_dest()->get_id();
        int e_id = e->get_id();
        
        pheromone p = { .current = 0.0f, .future = 0.0f };
        if (e->pheromone_exists(time)) {
            p = e->get_pheromone(time);
        }
        
        total_prob += calculate_heuristic(n_nodeid, dest_id, e->get_distance(), p.current, max_pheromone);
        
        if (total_prob/total >= prob) {
            return e;
        }
    }
    
    return nullptr;
}

double heuristic_selector::calculate_heuristic(int node_id, int dest_id, int e_dist, double ph, double max_pheromone) {
    float d = distance(node_id, dest_id, e_dist)*1000;
    
    if (ph == 0.0f) {
        return pow(d, PHI);
    }
    
    return pow((ph/max_pheromone), ALPHA) * pow(d, BETA);
}

float heuristic_selector::distance(int node_id, int dest_id, int e_dist) {
    int distance = e_dist + d_map->get_edge_weight(node_id, dest_id);
    
    float t_d = distance / d_map->get_max_dj_distance();

    return (1.0f - ((t_d >= 1.0f) ? distance / (e_dist + d_map->get_max_dj_distance() + 1) : t_d));
}
