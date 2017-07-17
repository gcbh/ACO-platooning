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
    ACO_new (graph *i_g, multimap< pair<int, int> , int> i_manifest, float i_alpha, float i_beta, float i_phi, float i_rho, bool i_debug, long seed);
    virtual ~ACO_new ();
    void    init(Dijkstra *dijkstra);
    int     iteration();
    
private:
    graph*                              g;
    Dijkstra*                           d_map;
    Randoms                             r;
    string                              RESULT_LOG_PATH;
    ofstream                            result_log;
    float                               RHO, ALPHA, BETA, PHI;
    multimap< pair<int, int> , int >    manifest;
    list<ant*>                          ants;
    string**                            print_route;
    int                                 num_ants, num_iters;
    bool                                DEBUG;
    
    void    set_prime_ant(list<string> manifest_route);
    void    evaporation();
    double  cost_evaluation(int max_duration);
    double  cost_based_num_ants(int num_of_ants);
    double  cost_per_tick(map< iPair, int > map_ant_count);
    void    reset_ants();
    void    log_results(int tick, int cost);
    void    log_rollback();
    
};

#endif /* ACO_new_hpp */
