//
//  Dijkstra.hpp
//
//  Created by Aditi Lath on 2017-06-09.
//  Copyright © 2017 Aditi Lath. All rights reserved.
//


#ifndef Dijkstra_hpp
#define Dijkstra_hpp

#include "../../utils/StringUtils.hpp"

#include <map>
#include <stdio.h>
#include <list>
#include <set>
#include <vector>
#include <iostream>

using namespace std;

struct graph_data {
    int src;
    int dest;
    double weight;
};

class Dijkstra {
public:
    Dijkstra();
    virtual ~Dijkstra();
    void init(list<graph_data> list, int node_count);
    void populate_from_dijkstra_file(string file_name, set< pair<int, int> > manifest_set);
    double get_edge_weight(int src, int dest);
    map<pair<int, int>, string> get_manifest_routes();
private:
    list< pair<double, int> > *edg;
    set<int> nodes;
    double **edge_weight;
    map<pair<int, int>, string> manifest_route;
    int num_of_nodes;
    void shortest_route( int src);
    void add_edge(int src, int dest, double weight);
    void print_src_data(int src, vector<double> wght, int route[]);
    void print_path(int route[], int j);
    vector<string> split(const string &s, char delim);
};

#endif 
