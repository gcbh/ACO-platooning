//
//  ACO_new.cpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#include "ACO_new.hpp"
#include <map>

ACO_new::ACO_new() {
    graph = new std::map<int, t_node>();
}
ACO_new::~ACO_new() {
}
