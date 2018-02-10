//
//  heuristic_selector.hpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-01-12.
//

#ifndef heuristic_selector_hpp
#define heuristic_selector_hpp

#include <stdio.h>
#include <math.h>

#include "t_edge.hpp"

#include "Dijkstra.hpp"
#include "Randoms.h"

/*
 *  heuristic_selector is designed to implement the logic of the heuristic.
 *  It is responsible for the selecting of the next edge that an ant will take.
 */

struct max_pheromones {
    float max, best_wait;
};

class heuristic_selector {
public:
    /*
     *  Constructor for the selector function.
     *  a - Alpha value
     *  b - Beta value
     *  p - Phi value
     *  seed - value used to set up
     *  map - pointer to Dijkstra data structure
     */
    heuristic_selector(float a, float b, float p, long seed, Dijkstra* map);
    t_edge* selected_edge(list<t_edge*> edges, int current_id, int dest_id, int time);
private:
    float ALPHA, BETA, PHI;
    Randoms probability;
    Dijkstra* d_map;
    
    max_pheromones get_max(list<t_edge*> edges, int time);
    double calculate_from_edge(t_edge* e, int dest_id, int time, float max_pheromone);
    pheromone get_pheromone(t_edge* e, int time);
    double calculate_heuristic(int node_id, int dest_id, int e_dist, double ph, float max_pheromone);
    float distance(int node_id, int dest_id, int e_dist);
};

#endif /* heuristic_selector_hpp */
