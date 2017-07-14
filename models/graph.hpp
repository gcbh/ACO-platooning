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
    void construct_graph(list<graph_data> i_edges);
    // TODO: t_node operator[](int);
    t_node* operator[](int);
    int get_num_nodes();
private:
    map<int, t_node*> *g_graph;
};

#endif /* graph_hpp */
