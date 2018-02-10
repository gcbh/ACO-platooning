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
    
    max_pheromones mp = get_max(edges, time);
    
    // calculate total of all heuristics for each edge
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        t_edge* e = (*it);
        
        total += calculate_from_edge(e, dest_id, time, mp.max);
    }
    
    // add "wait" heuristic to total
    total += calculate_heuristic(current_id, dest_id, 0, mp.best_wait, mp.max);
    
    double prob = probability.Uniforme();
    
    // pick edge to traverse
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        t_edge* e = (*it);
        
        total_prob += calculate_from_edge(e, dest_id, time, mp.max);
        
        if (total_prob/total >= prob) {
            return e;
        }
    }
    
    return nullptr;
}

// get maximum pheromone of all the edges available including "wait" pheromone
max_pheromones heuristic_selector::get_max(list<t_edge*> edges, int time) {
    max_pheromones mp = { .max=0.0f, .best_wait=0.0f };
    
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        t_edge* e = (*it);
        pheromone p = get_pheromone(e, time);
        
        if (p.current > mp.max) mp.max = p.current;
        
        if (p.future > mp.max) mp.max = p.future;
        
        if (p.future > mp.best_wait) mp.best_wait = p.future;
    }
    return mp;
}

double heuristic_selector::calculate_from_edge(t_edge* e, int dest_id, int time, float max_pheromone) {
    int n_nodeid = e->get_dest()->get_id();
    
    pheromone p = get_pheromone(e, time);
    
    return calculate_heuristic(n_nodeid, dest_id, e->get_distance(), p.current, max_pheromone);
}

pheromone heuristic_selector::get_pheromone(t_edge* e, int time) {
    pheromone p = { .current = 0.0f, .future = 0.0f };
    if (e->pheromone_exists(time)) {
        p = e->get_pheromone(time);
    }
    return p;
}

double heuristic_selector::calculate_heuristic(int node_id, int dest_id, int e_dist, double ph, float max_pheromone) {
    float d = distance(node_id, dest_id, e_dist);
    
    if (ph == 0.0f) {
        return pow(d, PHI);
    }
    
    return pow((ph/max_pheromone), ALPHA) * pow(d, BETA);
}

float heuristic_selector::distance(int node_id, int dest_id, int e_dist) {
    int distance = e_dist + d_map->get_edge_weight(node_id, dest_id);
    float max = d_map -> get_max_dj_distance();
    
    if (distance > max) {
        max += e_dist;
    }
    
    if (distance == max) {
        max += 1;
    }
    
    return 1.0f - (distance / max);
}
