//
//  graph.cpp
//  models
//
//  Created by Priya Bibra on 2017-06-08.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "graph.hpp"

graph::graph() {
    //g_graph = map<int, t_node>();
}

graph::~graph() {
}

void graph::construct_graph(list<graph_data> i_edges) {
    int speed = 100; //fixed speed for now
    for (list<graph_data>::iterator it = i_edges.begin(); it != i_edges.end(); ++it) {
        t_node *src_node = new t_node(it->src);
        t_node *dest_node = new t_node(it->dest);

        //create two way edges
        t_edge *edge_sd = new t_edge(dest_node, it->weight, speed);
        t_edge *edge_ds = new t_edge(src_node, it->weight, speed);

        src_node->add_edge(edge_sd);
        dest_node->add_edge(edge_ds);

        //src node is not yet in the graph
        if (g_graph.count(it->src) == 0) {
            g_graph.insert(make_pair(it->src, *src_node));
        } else {
            // map<int, t_node>::iterator itr = g_graph.find(it->src);
            // t_node *node = NULL;
            // if (itr != g_graph.end()) {
            //     node = &((*itr).second);
            // }
            t_node *node = (*this)[it->src];
            node->add_edge(edge_sd);
        }

        //dest node is not yet in the graph
        if (g_graph.count(it->dest) == 0) {
            g_graph.insert(make_pair(it->dest, *dest_node));
        } else {
            // map<int, t_node>::iterator itr = g_graph.find(it->dest);
            // t_node *node = NULL;
            // if (itr != g_graph.end()) {
            //     node = &((*itr).second);
            // }
            t_node *node = (*this)[it->dest];
            node->add_edge(edge_ds);
        }
    }
}

t_node* graph::operator[](int i_node_id) { 
     map<int, t_node>::iterator itr = g_graph.find(i_node_id);
    t_node *node = NULL;
    if (itr != g_graph.end()) {
        node = &((*itr).second);
    }

    return node;
}