//
//  t_edge.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#include "t_edge.hpp"

t_edge::t_edge(t_node i_dest, int i_distance, int i_speed) {
    dest = i_dest;
    distance = i_distance;
    speed = i_speed;
    max_tick = 0;
    max_phermone = 0.0;
    phermone_at = new std::map<float>();
}
