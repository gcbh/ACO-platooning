//
//  t_node.hpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#ifndef t_node_hpp
#define t_node_hpp

#include "t_edge.hpp"

#include <stdio.h>

class t_edge;

class t_node {
public:
    t_node(int i_id, int n_edges);
    ~t_node();
    int edge_number();
    
private:
    int id, NUMBEROFEDGES;
    t_edge *edges[];
};

#endif /* t_node_hpp */
