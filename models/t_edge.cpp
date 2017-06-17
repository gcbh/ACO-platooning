//
//  t_edge.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#include "t_edge.hpp"

using namespace std;

t_edge::t_edge(t_node *i_dest, int i_distance, int i_speed) {
    dest = i_dest;
    distance = i_distance;
    speed = i_speed;
    phermone_at = new map<int, phermone>();
}

t_edge::~t_edge() {
    delete phermone_at;
}
