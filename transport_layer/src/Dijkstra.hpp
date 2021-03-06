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
#include <unordered_set>
#include <vector>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../../utils/StringUtils.hpp"
#include "../../models/graph.hpp"
#include "../../models/manifest.hpp"
#include "../../models/map_data.hpp"


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
    void            init(map_data map);
    void            populate_from_dijkstra_file(string file_name, manifest manifest_map, map<pair<int, int>,
     string>* gp_map);
    int             get_edge_weight(int src, int dest);
    float           get_max_dj_distance();
    vector<string>  get_manifest_routes();
    
private:
    vector< pair<int, int> >* edg;
    unordered_set<int>      nodes;
    int**                   edge_weight;
    vector<string>          manifest_route;
    int                     num_of_nodes;
    float                   max_distance;
    
    void shortest_route(int src);
    void add_edge(int src, int dest, int weight);
    void print_src_data(int src, vector<int> wght, int route[]);
    void print_path(int route[], int j);
};

#endif 
