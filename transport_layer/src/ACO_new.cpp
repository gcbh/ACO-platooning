//
//  ACO_new.cpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#include "ACO_new.hpp"

using namespace std;

const float avg_prcnt_fuel_saving_by_middle = 4.1;
const float avg_prcnt_fuel_saving_by_last = 6.1;

ACO_new::ACO_new(graph *i_g, int i_num_iterations, multimap< pair<int, int> , int> i_manifest) {
    g = i_g;
    r = new Randoms(21);
    num_iterations = i_num_iterations;
    RHO = 0.2;
    ALPHA = 0.5;
    BETA = 0.5;
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
    double cost;
    for (multimap< pair<int, int> , int>::iterator it = manifest.begin(); it != manifest.end(); ++it) {
        int src = it->first.first;
        int dest = it->first.second;
        //ant *a = new ant((*g)[src], dest);
        ant a((*g)[src], dest, ALPHA, BETA, r);
        ants->push_back(a);
    }
    int tick = 0;
    int max_tick = 0;
    for(int i = 1; i == num_iterations; i++) {
        for (list<ant>::iterator it = ants->begin(); it != ants->end(); ++it) {
            //while not reached destination call nextnode
            while(!it->hasReachedDestination()) {
                it->next_node(tick);
                tick++;
            }

            max_tick < tick ? max_tick = tick : max_tick; //needed for cost evaluation            
            tick = 0;
        }

        //TODO: loop through each ant and print their ordered path?
        evaporation();
        cost = cost_evaluation(max_tick);
        max_tick = 0;
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
    return 0.0;
}

double ACO_new::cost_per_tick(map< int, set<int> > *edge_with_ants) {
    map< int, set<int> >::iterator itm;
    double cost_per_tick = 0;
    for (itm = edge_with_ants->begin(); itm != edge_with_ants->end(); ++itm) {
        set<int> value_set = itm->second;
        int num_of_ants = value_set.size();
        cost_per_tick += cost_based_num_ants(num_of_ants);
    }
    return cost_per_tick;
}

double ACO_new::cost_based_num_ants(int num_of_ants) {

    int num_at_middle = num_of_ants - 2;
    double cost_for_middle = num_at_middle * (1 - (avg_prcnt_fuel_saving_by_middle/100));
    double cost_for_last = (1 - (avg_prcnt_fuel_saving_by_last/100));

    return 1 + cost_for_middle + cost_for_last;
}
