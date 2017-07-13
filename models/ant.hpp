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
#include <vector>
#include <set>

#include "base_ant.hpp"
#include "t_node.hpp"
#include "../transport_layer/src/Randoms.h"

using namespace std;

typedef pair<int, int> iPair;

class ant : base_ant {
public:
    // TODO: must pass in destination node
    
    // TODO: must decide if queue or set is best option
    ant(t_node *first, int i_dest, float i_alpha, float i_beta, Randoms *i_r);
    ~ant();
    void next_node(int time);
    iPair cost_node(int time);
    bool hasReachedDestination();
    vector<int> get_ordered_path();
private:
    
    t_node *current;
    int dest, counter;
    float alpha, beta;
    Randoms *probability;
    set<int> past_nodes;
    vector<int> ordered_path;
};
#endif /* ant_hpp */
