//
//  ga_objective.cpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-02-02.
//

#include "ga_objective.hpp"
template <typename T>

static std::vector<T> ga_objective::Objective(const std::vector<T>& x) {
    config c;
    
    heuristic_selector* sel = new heuristic_selector(conf.getAlpha(), conf.getBeta(), conf.getPhi(), seed, dijkstra);
    
    cost_function* cost = new cost_function();
}
