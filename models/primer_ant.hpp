//
//  primer_ant.hpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#ifndef primer_ant_hpp
#define primer_ant_hpp

#include <stdio.h>
#include <queue>

#include "t_edge.hpp"
#include "base_ant.hpp"

using namespace std;

class primer_ant : base_ant {
public:
    //TODO: constructor must take in destination node
    primer_ant(queue<int> *i_nodes, t_node *first);
    void next_node(int time);
};

#endif /* primer_ant_hpp */
