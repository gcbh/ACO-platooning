//
//  ant.hpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#ifndef ant_hpp
#define ant_hpp

#include <stdio.h>
#include <string>
#include <math.h>
#include <vector>
#include <set>

#include "base_ant.hpp"
#include "t_node.hpp"
#include "../transport_layer/src/Randoms.h"
#include "../transport_layer/src/Dijkstra.hpp"


using namespace std;

typedef pair<int, int> iPair;

class ant : base_ant {
public:
    ant(t_node* first, Dijkstra* i_d_map, int i_dest, float i_alpha, float i_beta, float i_phi, Randoms* i_r);
    ~ant();
    void next_node(int time);
    iPair cost_node(int time);
    vector<int> get_ordered_path();
    bool has_reached_destination();

private:
    t_node *current;
    Dijkstra* d_map;
    int dest, counter;
    float ALPHA, BETA, PHI;
    Randoms *probability;
    set<int> past_nodes;
    vector<int> ordered_path;
    
    double calculate_heuristic(int node_id, float ph);
    void set_node(t_edge* e);
};
#endif /* ant_hpp */
