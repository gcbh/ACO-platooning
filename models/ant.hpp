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

class ant: public base_ant {
public:
    ant(t_node* first, int i_dest, float d, heuristic_selector* sel);
    ~ant();
    path            next_node(int time);
    bool            void_route();
    bool            has_concluded();
private:
    heuristic_selector*   selector;
    float                 DELTA;
    unordered_set<int>    past_edges;
    bool                  v_route;
    list<t_edge*>         avail_edges();
    double calculate_heuristic(int node_id, int e_dist, float ph);
};
#endif /* ant_hpp */
