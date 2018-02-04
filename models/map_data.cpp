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
    
    if (edges.size() > 0) {
        for (list<graph_edge>::iterator it= edges.begin(); it != edges.end(); ++it ) {
            if ((*it).src != edge.src && (*it).dest != edge.dest) {
                edges.push_back(edge);
            }
        }
    } else {
        edges.push_back(edge);
    }
    

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
