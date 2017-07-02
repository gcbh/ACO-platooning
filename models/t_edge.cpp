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
    
}

int t_edge::get_time_to_cross() {
    return time_to_cross;
}
