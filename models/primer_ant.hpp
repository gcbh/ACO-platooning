//
//  primer_ant.hpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#ifndef primer_ant_hpp
#define primer_ant_hpp

#include "base_ant.hpp"
#include "t_edge.hpp"
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <queue>

using namespace std;

class primer_ant: base_ant {
public:
    primer_ant(t_node *first, vector<string> route_path);
    bool            has_reached_destination();
    queue<t_node*>  get_ordered_path();
    void            init_cost();
    void            next_node(int time);

private:
    vector<string>  route;
};

#endif /* primer_ant_hpp */
