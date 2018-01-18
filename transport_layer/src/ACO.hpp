//
//  ACO.hpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#ifndef ACO_hpp
#define ACO_hpp

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

#include "Randoms.h"
#include "Dijkstra.hpp"
#include "heuristic_selector.hpp"
#include "cost_function.hpp"

#include "../../utils/StringUtils.hpp"
#include "../../models/config.hpp"
#include "../../models/t_node.hpp"
#include "../../models/graph.hpp"
#include "../../models/manifest.hpp"
#include "../../models/t_edge.hpp"
#include "../../models/primer_ant.hpp"
#include "../../models/ant.hpp"

using namespace std;

class ACO {
public:
    ACO (graph *i_g, manifest i_manifest, config i_config, heuristic_selector* i_sel, cost_function* i_j);
    virtual ~ACO ();
    void    init(Dijkstra *dijkstra);
    int     iteration();
    vector<string>** result();
    
private:
    graph*                              g;
    Dijkstra*                           d_map;
    config                              conf;
    heuristic_selector*                 sel;
    cost_function*                      j;
    vector<string>**                    output;
    string                              RESULT_LOG_PATH;
    ofstream                            result_log;
    manifest                            manifest_data;
    list<base_ant*>                     ants;
    int                                 num_iters;
    double                              prev_cost;
    
    void    set_prime_ant(list<string> manifest_route);
    void    evaporate_update_future_pheromones(int ticks);
    void evaporation(unordered_set<int> traversed, double mag, int ticks);
    double  evaluation(int max_duration);
    void    reset_ants();
    void    init_log();
    void    log_tick(int tick, list<string> segments);
    void    log_cost(double cost);
};

#endif /* ACO_hpp */
