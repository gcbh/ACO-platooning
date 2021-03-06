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
    g_graph = new map<int, t_node*>();
}

graph::~graph() {
    for (map<int, t_node*>::iterator itr = g_graph->begin(); itr != g_graph->end(); ++itr) {
        delete itr->second;
    }
    delete g_graph;
}

void graph::construct_graph(map_data data) {
    set_nodes(data.getNodes());
    int speed = 75; //fixed nominal average speed on every edge 
    int edge_id = 0;
    for (list<graph_edge>::iterator it = data.begin(); it != data.end(); ++it) {
        t_node *src_node; //= new t_node(it->src);
        t_node *dest_node; //= new t_node(it->dest);

        int src = it->src;
        int dest = it->dest;
        //src node is not yet in the graph
        if (g_graph->count(src) == 0) {
            src_node = new t_node(it->src);
            g_graph->insert(make_pair(it->src, src_node));
        } else {
            src_node = (*this)[it->src];
        }

        //dest node is not yet in the graph
        if (g_graph->count(dest) == 0) {
            dest_node = new t_node(it->dest);
            g_graph->insert(make_pair(it->dest, dest_node));
        } else {
            dest_node = (*this)[it->dest];
        }
        
        //create two way edges
        t_edge *edge_sd = new t_edge(edge_id, dest_node, it->weight, speed);
        edge_id++;
        t_edge *edge_ds = new t_edge(edge_id, src_node, it->weight, speed);
        edge_id++;
        
        src_node->add_edge(edge_sd);
        dest_node->add_edge(edge_ds);
    }
}

void graph::set_nodes(unordered_set<int> node_ids) {
    nodes = node_ids;
}

unordered_set<int> graph::get_nodes() {
    return nodes;
}

t_node* graph::operator[](int i_node_id) {
    return (*g_graph)[i_node_id];
}

int graph::get_num_nodes() {
    return g_graph->size();
}
