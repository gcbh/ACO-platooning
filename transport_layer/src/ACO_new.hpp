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
#include <iomanip>
#include "../../utils/StringUtils.hpp"
#include "../../models/t_node.hpp"
#include "../../models/graph.hpp"
#include "../../models/t_edge.hpp"
#include "../../models/primer_ant.hpp"
#include "../../models/ant.hpp"
#include "Dijkstra.hpp"


class ACO_new {
public:
    ACO_new (graph *i_g, multimap< pair<int, int> , int> i_manifest, float i_alpha, float i_beta, float i_delta, float i_lambda, float i_phi, float i_rho, bool i_debug, long seed);
    virtual ~ACO_new ();
    void    init(Dijkstra *dijkstra);
    int     iteration();
    
private:
    graph*                              g;
    Dijkstra*                           d_map;
    Randoms                             r;
    string                              RESULT_LOG_PATH;
    ofstream                            result_log;
    float                               RHO, ALPHA, BETA, DELTA, LAMBDA, PHI;
    multimap< pair<int, int> , int >    manifest;
    list<base_ant*>                     ants;
    int                                 num_ants, num_iters;
    bool                                DEBUG;
    double                              prev_cost;
    
    void    set_prime_ant(list<string> manifest_route);
    void    evaporate_update_future_pheromones(int ticks);
    void    rollback_evaporation(int tick, float value);
    double  cost_evaluation(int max_duration, list<base_ant*> base_ants);
    double  cost_based_num_ants(int num_of_ants);
    double  cost_per_tick(map< iPair, int > map_ant_count);
    void    reset_ants();
    void    log_results(int tick, int cost, string** print_route);
    void    log_rollback(int node_id);
    
};

#endif /* ACO_new_hpp */
