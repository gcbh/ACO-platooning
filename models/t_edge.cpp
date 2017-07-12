//
//  t_edge.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#include "t_edge.hpp"

using namespace std;

t_edge::t_edge(t_node *i_dest, double i_distance, int i_speed) {
    dest = i_dest;
    distance = i_distance;
    speed = i_speed;
    phermone_at = new map<int, phermone>();
    time_to_cross = round(i_distance/speed);
}

t_edge::~t_edge() {
    delete phermone_at;
//    if (dest != nullptr) {
//        delete dest;
//    }
}

t_node* t_edge::get_dest() {
    return dest;
}

phermone t_edge::get_phermone(int time) {
    return (*phermone_at)[time];
}

void t_edge::update_phermone(int time, int value) {
    phermone old_p = get_phermone(time);
    phermone new_p;

    new_p.current = value;
    new_p.future = old_p.future;

    phermone_at->insert(make_pair(time, new_p));

    update_future_pheromone(time, value); 
}

int t_edge::get_time_to_cross() {
    return time_to_cross;
}

//  update all previous times' future values to reflect new changes for current time.
//  eg. if time = 2 was updated in update_phermone to hold a value of 3
//  future values of pheromones at time 0 and 1 should update if 3 is greater 
//  than their current future projections.
void t_edge::update_future_pheromone(int time, int value) {
    phermone p;
    for (int i = 0; i < time; i++) {
        p = get_phermone(i);
        if (p.future < value) {
            p.future = value;
        }
    }
}
