//
//  ACO_new.hpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#ifndef ACO_new_hpp
#define ACO_new_hpp

#include "Randoms.h"
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../utils/StringUtils.hpp"
#include "../../models/t_node.hpp"
#include "../../models/graph.hpp"
#include "../../models/t_edge.hpp"
#include "../../models/primer_ant.hpp"
#include "../../models/ant.hpp"

class ACO_new {
public:
    ACO_new (graph *i_g, int i_num_iterations, multimap< pair<int, int> , int> i_manifest);
    virtual ~ACO_new ();
    void init(Dijkstra *dijkstra);
private:
    graph* g;
    Dijkstra* d_map;
    Randoms r;
    int num_iterations;
    ofstream result_log;
    string RESULT_LOG_PATH;
    float RHO, ALPHA, BETA, PHI;

    multimap< pair<int, int> , int> manifest;
    ant *ants;
    void set_prime_ant(list<string> manifest_route);
    void iteration();
    void delta_pheromone(int time, t_edge *edge);
    void evaporation();
    double cost_evaluation(int max_duration);
    double cost_based_num_ants(int num_of_ants);
    double cost_per_tick(map< int, set<int> > *edge_with_ants);
    void reset_ants();
};

#endif /* ACO_new_hpp */
