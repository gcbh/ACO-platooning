//
//  graph.hpp
//  models
//
//  Created by Priya Bibra on 2017-06-08.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include <iostream>
#include <list>
#include <map>
#include <unordered_set>
#include "map_data.hpp"
#include "t_node.hpp"
#include "t_edge.hpp"

struct graph_data {
    int src;
    int dest;
    int weight;
};

class graph {
public:
    graph();
    ~graph();
    void                construct_graph(map_data data);
    t_node*             operator[](int);
    int                 get_num_nodes();
    void                set_nodes(unordered_set<int> node_ids);
    unordered_set<int>  get_nodes();
private:
    map<int, t_node*>   *g_graph;
    unordered_set<int>  nodes;
};

#endif /* graph_hpp */
