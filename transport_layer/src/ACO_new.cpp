//
//  ACO_new.cpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#include "ACO_new.hpp"
#include <map>

ACO_new::ACO_new(graph *i_g, int i_num_iterations) {
    g = i_g;
    num_iterations = i_num_iterations;
    RHO = 0.2;
}
ACO_new::~ACO_new() {
}

void ACO_new::iteration() {

}

void ACO_new::delta_pheromone(int time, t_edge *edge) {
    int new_value = edge->get_phermone(time).current + 1;
    edge->update_phermone(time, new_value);
}

void ACO_new::evaporation() {
    int new_value = 0;
    vector<t_edge*> edges;
    for (int i = 0; i < g->get_num_nodes(); i++) {
        edges = (*g)[i]->get_edges();
        for (int j = 0; j < edges.size(); j++) {
            for (int k = 0; k < edges[j]->get_time_to_cross(); k++) {
                new_value = (1-RHO)*edges[j]->get_phermone(k).current;
                edges[j]->update_phermone(k, new_value);
            }
        }
        edges.clear();
    }
}