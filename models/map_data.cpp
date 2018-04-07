//
//  map_data.cpp
//  models
//
//  Created by Geoffrey Heath on 2018-01-11.
//  Copyright © 2018 University of Waterloo. All rights reserved.
//

#include "map_data.hpp"

map_data::map_data() {  }

void map_data::insert(int src, int dest, int distance) {
    nodes.insert(src);
    nodes.insert(dest);
    
    graph_edge edge;
    edge.src = src;
    edge.dest = dest;
    edge.weight = distance;
    
    if (!edge_exists(edge)) {
        edges.push_back(edge);
        existing_edges.insert(make_pair(src, dest));
    }
}

bool map_data::edge_exists(graph_edge edge) {
    pair<int, int> p = make_pair(edge.src, edge.dest);
    pair<int, int> p_reverse = make_pair(edge.dest, edge.src);
    
    if (existing_edges.find(p) != existing_edges.end() || existing_edges.find(p_reverse) != existing_edges.end()) {
        return true;
    }

    return false;
}

int map_data::node_count() {
    return nodes.size();
}

unordered_set<int> map_data::getNodes() {
    return nodes;
}

list<graph_edge>::iterator map_data::begin() {
    return edges.begin();
}

list<graph_edge>::iterator map_data::end() {
    return edges.end();
}

void map_data::data(string line, map_data* m) {
    stringstream   linestream(line);
    int            src, dest, distance;
    string         src_name, dest_name;
    
    linestream >> src >> src_name >> dest >> dest_name >> distance;
    
    m->insert(src, dest, distance);
}

void map_data::dist_data(string line, map_data* m) {
    stringstream   linestream(line);
    int            src, dest, distance;
    string         src_name, dest_name;
    double         src_lat, src_long, dest_lat, dest_long;
    
    linestream >> src >> src_name >> src_lat >> src_long >> dest >> dest_name >> dest_lat >> dest_long >> distance;
    
    m->insert(src, dest, distance);
}
