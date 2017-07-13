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
    //edges = new vector<t_edge*>();
}

int t_node::get_id() {
    return id;
}

int t_node::edge_number() {
    return edges.size();
}

t_node::~t_node() {
    for(vector<t_edge*>::iterator itr = edges.begin(); itr != edges.end(); ++itr) {
        delete *itr;
    }
}

void t_node::add_edge(t_edge *edge) {
    edges.push_back(edge);
}

t_edge* t_node::operator[](int i) {
    return edges[i];
}

vector<t_edge*> t_node::get_edges() {
    return edges;
}

t_edge* t_node:: get_edge(int dest_id) {
    vector<t_edge*> v_edges = get_edges();
    for (int i = 0; i < v_edges.size(); i++) {
        if (dest_id == v_edges[i]->get_dest()->get_id()) {
            return v_edges[i];
        }
    }
    return NULL;
}
