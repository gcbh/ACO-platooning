//
//  Dijkstra.hpp
//
//  Created by Aditi Lath on 2017-06-09.
//  Copyright © 2017 Aditi Lath. All rights reserved.
//


#ifndef Dijkstra_hpp
#define Dijkstra_hpp

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
    void populate_weight(string file_name);
    double get_edge_weight(int src, int dest);
private:
    list< pair<double, int> > *edg;
    set<int> nodes;
    double **edge_weight;
    // vector< vector<int> > edge_weight;
    int num_of_nodes;
    void shortest_route( int src);
    void add_edge(int src, int dest, double weight);
    void print_src_data(int src, vector<double> wght, int route[]);
    void print_path(int route[], int j);
    vector<string> split(const string &s, char delim);
};

#endif 
