//
//  ACO_new.cpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#include "ACO_new.hpp"
#include <map>

#include "../../utils/StringUtils.hpp"

ACO_new::ACO_new(graph *i_g, int i_num_iterations) {
    g = i_g;
    num_iterations = i_num_iterations;
    RHO = 0.2;
}
ACO_new::~ACO_new() {
}

void ACO_new:: init(Dijkstra *dijkstra) {
    set_prime_ant(dijkstra->get_manifest_routes());
}

void ACO_new:: set_prime_ant(map<pair<int, int>, string> manifest_route) {
    
    map<pair<int, int>, string>:: iterator it;
    StringUtils *s; // To Do: Figure out a way to use functions without instantiating a class
    for (it = manifest_route.begin(); it != manifest_route.end(); it++) {
        long tick = 0;
        string route = it->second;
        vector<string> columns = s->split(route, ' ');
    }
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