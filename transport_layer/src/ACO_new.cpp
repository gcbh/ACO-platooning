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
    double cost;
    for (multimap< pair<int, int> , int>::iterator it = manifest.begin(); it != manifest.end(); ++it) {
        int src = it->first.first;
        int dest = it->first.second;
        //ant *a = new ant((*g)[src], dest);
        ants->push_back(*(new ant((*g)[src], dest)));
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
    list<ant> final_ant_list = *ants;
    vector<int> *ants_previous_node = new vector<int>[final_ant_list.size()];
    int count = 0;
    double total_cost = 0;

    for (list<ant>::iterator it = final_ant_list.begin(); it != final_ant_list.end(); ++it) {
        ants_previous_node[count].push_back(it->get_ordered_path().front());
        it->get_ordered_path().pop();
        count++;
    }

    for (int i = 0; i < max_duration; i++) {
        map< int, set<int> > *map_ant_count = new map< int, set<int> >();
        count = 0;
        for (list<ant>::iterator it = final_ant_list.begin(); it != final_ant_list.end(); ++it) {
            if (it->get_ordered_path().size() != 0 ) {
                if (it->get_ordered_path().front() == (*ants_previous_node)[count])
                    break;
                t_node *previous = (*g) [(*ants_previous_node)[count]];
                t_edge *cur_edge = previous->get_edge(it->get_ordered_path().front());

                // if edge in map update the set of ants to append new ant
                if (map_ant_count->count(cur_edge->get_id())) {
                    set<int> cur_edge_set = map_ant_count->find(cur_edge->get_id())->second;
                    cur_edge_set.insert(count);
                    map_ant_count->insert(make_pair(cur_edge->get_id(), cur_edge_set));
                }

                ants_previous_node[count].push_back(it->get_ordered_path().front());
                it->get_ordered_path().pop();
            }
            count++;
        }

        // cost calculation per tick
        total_cost += cost_per_tick(map_ant_count);

        // TO DO : Modify total cost to take distance into consideration
    }
    return total_cost;
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
