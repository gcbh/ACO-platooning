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
#include <queue>
#include <unordered_set>

#include "base_ant.hpp"
#include "t_node.hpp"
#include "../transport_layer/src/heuristic_selector.hpp"

using namespace std;
typedef pair<int, int> iPair;

class ant: public base_ant {
public:
    ant(t_node* first, int i_dest, float d, heuristic_selector* sel);
    ~ant();
    void            next_node(int time);
    void            roll_back(int time, float magnitude);
    bool            has_reached_destination();
    void            init_cost();
    bool            void_route();
    queue<t_node*>  get_ordered_path();
    iPair           cost_node(int time);

private:
    heuristic_selector*   selector;
    float                 DELTA;
    unordered_set<int>    past_nodes;
    bool                  v_route;
    
    list<t_edge*>         avail_edges();
    double calculate_heuristic(int node_id, int e_dist, float ph);
};
#endif /* ant_hpp */
