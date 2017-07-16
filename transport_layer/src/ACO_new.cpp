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
    ALPHA = 0.2;
    BETA = 0.8;
    PHI = 0.1;
    manifest = i_manifest;
    RESULT_LOG_PATH = "../results.log";
    result_log.open(RESULT_LOG_PATH); 
    freopen(RESULT_LOG_PATH.c_str(), "w", stdout);
}

ACO_new::~ACO_new() {
    fclose(stdout);
    result_log.close();
    delete g;
    delete d_map;

    for (int i = 0; i < num_ants; i++)
        delete [] print_route[i];
    
    delete [] print_route;
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
    int tick;
    bool endIteration = true;

    for(int i = 1; i <= num_iterations; i++) {
        cout << "ITERATION NUMBER " << i << "\n";
        tick = -1;
        bool ant_void = false;
        do {
            endIteration = true;
            tick++;
            for (list<ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
                //if ant has not reached destination call nextnode
                if (!(*it)->has_reached_destination()) {
                    (*it)->next_node(tick);   
                    endIteration = false;   
                }
                
                if ((*it)->void_route()) {
                    endIteration = true;
                    ant_void = true;
                }
            }
        } while(!endIteration);

        if (ant_void) {
            cout << "ROLLBACK\n";
            for (int t = 0; t <= tick; ++t) {
                for (list<ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
                    (*it)->roll_back(t);
                }
            }
            reset_ants();
            continue;
        }
        
        evaporation();
        for (list<ant*>::iterator itr = ants.begin(); itr != ants.end(); ++itr) {
            (*itr)->init_cost();
        }
        cost = cost_evaluation(tick);
        cout<<setw(20);
        list<string> r = d_map->get_manifest_routes();
        int ant_num = 0;
        for (list<string>::iterator it = r.begin(); it != r.end(); ++it) {
            cout << "Ant" << ant_num << " " << *it << setw(10);;
            ant_num++;
        }
        
        for (int i = 0; i <= tick; i++) {
            cout<< "\n" << "tick" << i << setw(20);
            for (int j = 0; j < num_ants; j++) {
                cout << print_route[j][i] << setw(20);
            }
        }
        reset_ants();
        
        cout << "Cost: " << cost << "\n";
    }

}

void ACO_new::evaporation() {
    int new_value = 0;
    vector<t_edge*> edges;
    for (int i = 0; i < g->get_num_nodes(); i++) {
        edges = (*g)[i]->get_edges();
        for (int j = 0; j < edges.size(); j++) {
            for (int k = 0; k < edges[j]->get_time_to_cross(); k++) {
                new_value = -RHO*edges[j]->get_pheromone(k).current;
                edges[j]->update_pheromone(k, new_value);
            }
        }
    }
}

double ACO_new::cost_evaluation(int max_duration) {
    double total_cost = 0;
    
    num_ants = ants.size();
    print_route = new string*[num_ants];
    for (int i = 0; i < num_ants; i++)
        print_route[i] = new string[max_duration+1];

    for (int tick = 0; tick <= max_duration; tick++) {
        map< iPair, int > map_ant_count;
        int ant_index = -1;
        for (list<ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
            ant_index++;
            if (!(*it)->has_reached_destination()) {
                iPair nodes_pair = (*it)->cost_node(tick);
                int pair_elem1 = get<0> (nodes_pair);
                int pair_elem2 = get<1> (nodes_pair);

                string route = to_string(pair_elem1) + "->" + to_string(pair_elem2);

                if (pair_elem1 == INF) {
                    route = "Transit";
                    print_route[ant_index][tick] = route;
                    continue;
                }
                
                if (pair_elem1 == pair_elem2) {
                    print_route[ant_index][tick] = route;
                    continue;
                }
                
                print_route[ant_index][tick] = route;

                int ant_count = 0;
                if (map_ant_count.count(nodes_pair)) 
                    int ant_count = map_ant_count.find(nodes_pair)->second;

                map_ant_count.insert(make_pair(nodes_pair, ant_count+1));
                
            } else {
                string f = "finished";
            }
            
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
        t_node* src = (*g) [get<0> (nodes_pair)];
        t_node* dest = (*g) [get<1> (nodes_pair)];
        t_edge* edg = src->get_edge(dest->get_id());
        int edge_cost = edg->get_distance();
        cost_per_tick += (edge_cost*cost_based_num_ants(num_of_ants));
    }
    return cost_per_tick;
}

double ACO_new::cost_based_num_ants(int num_of_ants) {

    double avg_cost = 1;
    if (num_of_ants == 1)
        return avg_cost;

    int num_at_middle = num_of_ants - 2;
    double cost_for_middle = 0;
    double cost_for_last = 0;

    if (num_at_middle > 0)
        cost_for_middle = num_at_middle * (avg_prcnt_fuel_saving_by_middle/100);

    cost_for_last = (avg_prcnt_fuel_saving_by_last/100);

    avg_cost = 1 - ((cost_for_middle + cost_for_last)/num_of_ants);

    return avg_cost;
}
