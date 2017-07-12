//
//  t_edge.hpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#ifndef t_edge_hpp
#define t_edge_hpp

#include "t_node.hpp"

#include <stdio.h>
#include <map>
#include <cmath>

using namespace std;

class t_node;

struct phermone {
    int current;
    int future;
};

class t_edge {
    
public:
    t_edge(t_node *i_dest, double i_distance, int i_speed);
    ~t_edge();
    t_node* get_dest();
    phermone get_phermone(int time);
    void update_phermone(int time, int value);
    int get_time_to_cross();
private:
    t_node *dest;
    int distance, speed, max_tick, time_to_cross;
    float max_phermone;
    map<int, phermone> *phermone_at;
    void update_future_pheromone(int time, int value);
};

#endif /* t_edge_hpp */
