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

void t_edge::update_pheromone(int time, float value) {
    pheromone old_p = get_pheromone(time);
    pheromone new_p;

    new_p.current = old_p.current + value;
    if (new_p.current < 0.0) {
        new_p.current = 0.0;
    }
    new_p.future = old_p.future;

    pheromone_at[time] = new_p;
}

int t_edge::get_time_to_cross() {
    return time_to_cross;
}

void t_edge::update_future_pheromone(int time, float value) {
    pheromone p;
    for (int i = 0; i < time; i++) {
        p = get_pheromone(i);
        pheromone new_p;
        new_p.current = p.current;
        new_p.future = value;
        pheromone_at[i] = new_p;
    }
}

void t_edge::evaporate(int time, float rho) {
    pheromone old_p = get_pheromone(time);
    pheromone new_p;
    
    new_p.current = (1-rho)*old_p.current;
    new_p.future = old_p.future;
    
    pheromone_at[time] = new_p;
}
