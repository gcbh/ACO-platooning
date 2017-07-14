//
//  t_edge.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#include "t_edge.hpp"

using namespace std;

t_edge::t_edge(int i_id, t_node *i_dest, int i_distance, int i_speed) {
    id = i_id;
    dest = i_dest;
    distance = i_distance;
    speed = i_speed;
    time_to_cross = round(i_distance/speed);
}

t_edge::~t_edge() { }

int t_edge::get_id() {
    return id;
}

t_node* t_edge::get_dest() {
    return dest;
}

pheromone t_edge::get_pheromone(int time) {
    return pheromone_at[time];
}

int t_edge::get_distance() {
    return distance;
}

bool t_edge::pheromone_exists(int time) {
    if (pheromone_at.find(time) == pheromone_at.end()) {
        return false;
    }
    return true;
}

void t_edge::update_pheromone(int time, int value) {
    pheromone old_p = get_pheromone(time);
    pheromone new_p;

    new_p.current = old_p.current + value;
    new_p.future = old_p.future;

    //pheromone_at.insert(make_pair(time, new_p));
    pheromone_at[time] = new_p;

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
    pheromone p;
    for (int i = 0; i < time; i++) {
        p = get_pheromone(i);
        if (p.future < value) {
            pheromone new_p;
            new_p.current = p.current;
            new_p.future = value;
            pheromone_at[time] = new_p;
        }
    }
}
