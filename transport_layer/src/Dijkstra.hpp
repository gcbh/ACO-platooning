//
//  Dijkstra.hpp
//
//  Created by Aditi Lath on 2017-06-09.
//  Copyright © 2017 Aditi Lath. All rights reserved.
//


#ifndef Dijkstra_hpp
#define Dijkstra_hpp

#include <stdio.h>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../utils/StringUtils.hpp"
#include "../../models/graph.hpp"

using namespace std;

struct manifest_data {
    int src;
    int dest;
    int duration;
};

class Dijkstra {
public:
    Dijkstra();
    virtual ~Dijkstra();
    void init(list<graph_data> list, int node_count);
    void populate_from_dijkstra_file(string file_name, multimap< pair<int, int>, int> manifest_map);
    int get_edge_weight(int src, int dest);
    list<string> get_manifest_routes();
private:
    list< pair<int, int> > *edg;
    set<int> nodes;
    int **edge_weight;
    list<string> manifest_route;
    int num_of_nodes;
    void shortest_route(int src);
    void add_edge(int src, int dest, int weight);
    void print_src_data(int src, vector<int> wght, int route[]);
    void print_path(int route[], int j);
};

#endif 
