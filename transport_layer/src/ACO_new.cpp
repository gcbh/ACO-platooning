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

ACO_new::ACO_new(graph *i_g, int i_num_iterations, multimap< pair<int, int> , int> i_manifest) {
    g = i_g;
    num_iterations = i_num_iterations;
    RHO = 0.2;
    list<ant> *ants = new list<ant>();
    manifest = i_manifest;
}
ACO_new::~ACO_new() {
}

void ACO_new:: init(Dijkstra *dijkstra) {
    set_prime_ant(dijkstra->get_manifest_routes());
}

void ACO_new:: set_prime_ant(list<string> manifest_route) {
    list<string>:: iterator it;
    for (it = manifest_route.begin(); it != manifest_route.end(); it++) {
        string route = *it;
        vector<string> nodes = split(route, ' ');
        t_node *start_from = (*g) [stoi(nodes[0])];
        
        primer_ant *ant = new primer_ant(start_from, nodes);

        ant->set_ant_path();
    }
}

void ACO_new::iteration() {
    for (multimap< pair<int, int> , int>::iterator it = manifest.begin(); it != manifest.end(); ++it) {
        int src = it->first.first;
        int dest = it->first.second;
        //ant *a = new ant((*g)[src], dest);
        ants->push_back(*(new ant((*g)[src], dest)));
    }
    for(int i = 0; i < num_iterations; i++) {
        int tick = 0;
        for (list<ant>::iterator it = ants->begin(); it != ants->end(); ++it) {
            //while not reached destination call nextnode
            while(!it->hasReachedDestination()) {
                it->next_node(tick);
            }
            tick++;
        }

        //TODO: loop through each ant and print their ordered path?

        //updateFuturePheromones(*ants);
        evaporation();
    }
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

double ACO_new::cost_evaluation(int max_duration) {
    list<ant> final_ant_list = *ants;
    return 0;
}