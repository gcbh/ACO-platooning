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
#include <vector>
#include <iterator>
#include <stdio.h>
#include <exception>

using namespace std;

class t_edge;

class t_node {
public:
    t_node(int i_id);
    ~t_node();
    int get_id();
    int edge_number();
    void add_edge(t_edge *edge);
    t_edge* operator[](int i);
    vector<t_edge*> get_edges();
    t_edge* get_edge(int dest_id); 
private:
    int id;
    vector<t_edge*> edges;
};

#endif /* t_node_hpp */
