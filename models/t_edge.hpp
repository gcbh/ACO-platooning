//
//  t_edge.hpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#ifndef t_edge_hpp
#define t_edge_hpp

#include "t_node.hpp"

#include <stdio.h>
#include <map>

class t_node;

class t_edge {
    
public:
    t_edge(t_node *i_dest, int i_distance, int i_speed);
    
private:
    t_node *dest;
    int distance, speed, max_tick;
    float max_phermone;
    std::map<int, float> phermone_at;
    
};

#endif /* t_edge_hpp */
