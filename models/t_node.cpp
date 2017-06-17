//
//  t_node.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#include "t_node.hpp"

using namespace std;

t_node::t_node(int i_id) {
    id = i_id;
    NUMBEROFEDGES = 0;
}

int t_node::edge_number() {
    return NUMBEROFEDGES;
}

t_node::~t_node() {
    edges.clear();
}

void t_node::add_edge(t_edge *edge) {
   edges.push_back(*edge);
   NUMBEROFEDGES = edges.size();
}
