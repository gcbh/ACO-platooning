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

class primer_ant: public base_ant {
public:
    primer_ant(t_node *first, vector<string> route_path, float i_p);
    path            next_node(int time);

private:
    vector<string>  route;
    float           primer_ph;
};

#endif /* primer_ant_hpp */
