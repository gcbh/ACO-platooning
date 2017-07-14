//
//  ACO_new.cpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#include "ACO_new.hpp"

using namespace std;

# define INF 0x3f3f3f3f
typedef pair<int, int> iPair;
const float avg_prcnt_fuel_saving_by_middle = 4.1;
const float avg_prcnt_fuel_saving_by_last = 6.1;

ACO_new::ACO_new(graph *i_g, int i_num_iterations, multimap< pair<int, int> , int> i_manifest) : r(21) {
    g = i_g;
    num_iterations = i_num_iterations;
    RHO = 0.2;
    ALPHA = 0.5;
    BETA = 0.5;
    PHI = 0.2;
    manifest = i_manifest;
    RESULT_LOG_PATH = "../results.log";
    result_log.open(RESULT_LOG_PATH); 
    freopen(RESULT_LOG_PATH.c_str(), "w", stdout);
}

ACO_new::~ACO_new() {
    fclose(stdout);
    result_log.close();
    delete g;
}

void ACO_new:: init(Dijkstra *dijkstra) {
    cout << setw(50) << "***BEGINNING ANT COLONY OPTIMIZATION***\n\n";
    cout << setw(50) << "------------------------------------------------------------\n\n";
    cout << setw(50) << "ANT PATHS" << endl;
    d_map = dijkstra;
    set_prime_ant(dijkstra->get_manifest_routes());
    reset_ants();
    iteration();
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

void ACO_new::reset_ants() {
    ants.clear();
    int src, dest;
    for (multimap< pair<int, int> , int>::iterator it = manifest.begin(); it != manifest.end(); ++it) {
        src = it->first.first;
        dest = it->first.second;
        ant *a = new ant((*g)[src], d_map, dest, ALPHA, BETA, PHI, &r);
        ants.push_back(a);
    }
}

void ACO_new::iteration() {
    double cost = 0;
    int tick = -1;
    int max_tick = 0;
    bool endIteration = true;

    for(int i = 1; i <= num_iterations; i++) {
        cout << "ITERATION NUMBER " << i;
        do {
            endIteration = true;
            tick++;
            for (list<ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
                //if ant has not reached destination call nextnode
                if (!(*it)->has_reached_destination()) {
                    (*it)->next_node(tick);   
                    endIteration = false;   
                }
            }
        } while(!endIteration);
        
        max_tick < tick ? max_tick = tick : max_tick; //needed for cost evaluation            

        evaporation();
        cost = cost_evaluation(max_tick);
        reset_ants();
        max_tick = 0;
    }

}

void ACO_new::delta_pheromone(int time, t_edge *edge) {
    int new_value = edge->get_pheromone(time).current + 1;
    edge->update_pheromone(time, new_value);
}

void ACO_new::evaporation() {
    int new_value = 0;
    vector<t_edge*> edges;
    for (int i = 0; i < g->get_num_nodes(); i++) {
        edges = (*g)[i]->get_edges();
        for (int j = 0; j < edges.size(); j++) {
            for (int k = 0; k < edges[j]->get_time_to_cross(); k++) {
                new_value = (1-RHO)*edges[j]->get_pheromone(k).current;
                edges[j]->update_pheromone(k, new_value);
            }
        }
        edges.clear();
    }
}

double ACO_new::cost_evaluation(int max_duration) {
    double total_cost = 0;

    for (int tick = 0; tick < max_duration; tick++) {
        map< iPair, int > map_ant_count;

        for (list<ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {

            iPair nodes_pair = (*it)->cost_node(tick);
            int pair_elem1 = get<0> (nodes_pair);
            int pair_elem2 = get<1> (nodes_pair);

            if (pair_elem1 == INF || pair_elem1 == pair_elem2) 
                continue;
            
            int ant_count = 0;
            if (map_ant_count.count(nodes_pair)) 
                int ant_count = map_ant_count.find(nodes_pair)->second;

            map_ant_count.insert(make_pair(nodes_pair, ant_count+1));

        }
        // cost calculation per tick
        total_cost += cost_per_tick(map_ant_count);
    }

    return total_cost;
}

double ACO_new::cost_per_tick(map< iPair, int > map_ant_count) {
    map< iPair, int >::iterator itm;
    double cost_per_tick = 0;
    for (itm = map_ant_count.begin(); itm != map_ant_count.end(); ++itm) {
        int num_of_ants = itm->second;

        iPair nodes_pair = itm->first;
        t_node src = *((*g) [get<0> (nodes_pair)]);
        t_node dest = *((*g) [get<1> (nodes_pair)]);
        t_edge edg = *(src.get_edge(dest.get_id()));
        int edge_cost = edg.get_distance();
        cost_per_tick += (edge_cost/cost_based_num_ants(num_of_ants));
    }
    return cost_per_tick;
}

double ACO_new::cost_based_num_ants(int num_of_ants) {

    int num_at_middle = num_of_ants - 2;
    double cost_for_middle = num_at_middle * (avg_prcnt_fuel_saving_by_middle/100);
    double cost_for_last = (avg_prcnt_fuel_saving_by_last/100);

    double avg_cost = 1 - ((cost_for_middle + cost_for_last)/num_of_ants);
    return avg_cost;
}
