//
//  t_node.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#include "t_node.hpp"

using namespace std;

t_node::t_node(int i_id, int n_edges) {
    id = i_id;
    NUMBEROFEDGES = n_edges;
}

t_node::~t_node() {
    for (int i = 0; i < NUMBEROFEDGES; i++) {
        delete edges[i];
    }
}

int edge_number() {
    return NUMBEROFEDGES;
}
