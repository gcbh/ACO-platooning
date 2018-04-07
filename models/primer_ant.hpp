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
#include <stdio.h>
#include <iostream>

using namespace std;

class primer_ant {
public:
    primer_ant(t_node *first, vector<string> route_path);
    void set_ant_path();
private:
    t_node *current;
    vector<string> route;
};

#endif /* primer_ant_hpp */
