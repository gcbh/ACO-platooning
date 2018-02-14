//
//  map_data.hpp
//  models
//
//  Created by Geoffrey Heath on 2018-01-11.
//  Copyright © 2018 University of Waterloo. All rights reserved.
//

#ifndef map_data_hpp
#define map_data_hpp

#include <stdio.h>
#include <list>
#include <unordered_set>
#include <set>
#include <iterator>
#include <sstream>

using namespace std;

struct graph_edge {
    int src, dest, weight;
};

class map_data {
public:
    map_data();
    void insert(int src, int dest, int distance);
    int node_count();
    list<graph_edge>::iterator begin();
    list<graph_edge>::iterator end();
    unordered_set<int> getNodes();
    
    static void data(string line, map_data* m);
    static void dist_data(string line, map_data* m);
private:
    bool edge_exists(graph_edge edge);
    unordered_set<int> nodes;
    set< pair<int, int> > existing_edges;
    list<graph_edge> edges;
};

#endif /* map_data_hpp */
