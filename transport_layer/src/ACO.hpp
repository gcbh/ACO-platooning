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
#include <unordered_set>
#include <cfloat>

#include "Randoms.h"
#include "Dijkstra.hpp"
#include "heuristic_selector.hpp"
#include "cost_function.hpp"
#include "logger.h"

#include "StringUtils.hpp"
#include "../../models/config.hpp"
#include "../../models/t_node.hpp"
#include "../../models/graph.hpp"
#include "../../models/manifest.hpp"
#include "../../models/t_edge.hpp"
#include "../../models/primer_ant.hpp"
#include "../../models/ant.hpp"

using namespace std;

struct position {
    int time, edge_id;
    
    bool operator == (position const& pos) const {
        return ((time == pos.time) && (edge_id == pos.edge_id));
    }
};

struct position_hash {
    std::size_t operator () (const position& pos) const {
        // modified Bernstein hash
        return (33 * pos.time) ^ pos.edge_id;
    }
};

class ACO {
public:
    ACO (graph *i_g, manifest i_manifest, config i_config, heuristic_selector* i_sel, cost_function* i_j, logger standard, logger cost, logger debug);
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
    manifest                            manifest_data;
    list<base_ant*>                     ants;
    int                                 num_iters;
    double                              lowest_cost;
    vector<string>**                    lowest_cost_route;
    logger                              std_out, cost_out, debug_log;
    
    void    set_prime_ant(list<string> manifest_route);
    void    evaporate_update_future_pheromones(int ticks);
    void    evaporation(unordered_set<position, position_hash> traversed, double mag, int ticks);
    double  evaluation(int max_duration);
    void    reset_ants();
    void    build_output(int ant_num, string act, vector<string>* action);
    double  path_failure_penalty();
    void    init_log();
    void    log_tick(int tick, vector<string> segments);
    void    save_lowest_cost_route();
};

#endif /* ACO_hpp */
