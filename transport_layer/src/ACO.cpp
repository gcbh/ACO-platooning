//
//  ACO.cpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#include "ACO.hpp"

# define INF 0x3f3f3f3f
typedef pair<int, int> iPair;
const float avg_prcnt_fuel_saving_by_middle = 4.1;
const float avg_prcnt_fuel_saving_by_last = 6.1;

<<<<<<< HEAD:transport_layer/src/ACO_new.cpp
ACO_new::ACO_new(graph *i_g, manifest i_manifest, config i_conf, heuristic_selector* i_sel) {
=======
ACO::ACO(graph *i_g, manifest i_manifest, config i_conf, long seed) : r(seed) {
>>>>>>> d5756c8df308278c969f430a3b895cdd7bd49d3a:transport_layer/src/ACO.cpp
    g = i_g;
    conf = i_conf;
    sel = i_sel;
    manifest_data = i_manifest;
    num_iters = 0;
    RESULT_LOG_PATH = "../results.log";
    result_log.open(RESULT_LOG_PATH);
    prev_cost = INF;
    freopen(RESULT_LOG_PATH.c_str(), "w", stdout);
}

ACO::~ACO() {
    fclose(stdout);
    result_log.close();
}

void ACO:: init(Dijkstra *dijkstra) {
    d_map = dijkstra;
    set_prime_ant(dijkstra->get_manifest_routes());

    cout << setw(50) << "***BEGINNING ANT COLONY OPTIMIZATION***\n\n";
    cout << setw(50) << "------------------------------------------------------------\n\n";
    cout << setw(50) << "ANT PATHS" << endl;
    
    reset_ants();
}

void ACO:: set_prime_ant(list<string> manifest_route) {
    int               tick = -1;
    list<string>::    iterator it;
    bool              endIteration;
    double            cost = 0;

    for (it = manifest_route.begin(); it != manifest_route.end(); it++) {
        string         route = *it;
        vector<string> nodes = split(route, ' ');
        t_node*        start_from = (*g) [stoi(nodes[0])];
        primer_ant*    p_ant = new primer_ant(start_from, nodes);

        ants.push_back(p_ant);
    }

    do {
        endIteration = true;
        tick++;
        for (list<base_ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
            //if ant has not reached destination call nextnode
            if (!(*it)->has_reached_destination()) {
                (*it)->next_node(tick);   
                endIteration = false;   
            }
        }
    } while(!endIteration);
    
    for (list<base_ant*>::iterator itr = ants.begin(); itr != ants.end(); ++itr) {
        (*itr)->init_cost();
    }
    
    cout << setw(50) << "*** DIJKSTRA COST ***\n\n";
    cout << setw(50) << "------------------------------------------------------------\n\n";
    
    cost = cost_evaluation(tick, ants);
    
}

void ACO::reset_ants() {
    for (list<base_ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
        delete (*it);
    }

    ants.clear();
    int src, dest;
    for (multimap< pair<int, int> , int>::iterator it = manifest_data.begin(); it != manifest_data.end(); ++it) {
        src = it->first.first;
        dest = it->first.second;
        ant *a = new ant((*g)[src], dest, conf.getDelta(), sel);
        ants.push_back(a);
    }
}

int ACO::iteration() {
    double  cost = 0;
    int     tick = -1;
    bool    ant_void = false;
    bool    endIteration;
    num_iters++;
    
    do {
        endIteration = true;
        tick++;
        for (list<base_ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
            //if ant has not reached destination call nextnode
            if (!(*it)->has_reached_destination()) {
                (*it)->next_node(tick);   
                endIteration = false;   
            }
            
            if (dynamic_cast<ant*>(*it)->void_route()) {
                if (conf.DEBUG()) log_rollback((*it)->get_ordered_path().back()->get_id());
                rollback_evaporation(tick, conf.getDelta());
                reset_ants();
                return -1;
                break;
            }
        }
    } while(!endIteration);
    
    for (list<base_ant*>::iterator itr = ants.begin(); itr != ants.end(); ++itr) {
        (*itr)->init_cost();
    }
    
    cost = cost_evaluation(tick, ants);
    
    if (prev_cost == INF) {
        prev_cost = cost;
    }
    if (cost < prev_cost) {
        rollback_evaporation(tick, -1.0f * conf.getLambda() * prev_cost / cost);
        prev_cost = cost;
    } else if (cost > prev_cost) {
        rollback_evaporation(tick, 1.0f * cost / prev_cost);
    }
    
    evaporate_update_future_pheromones(tick);
//    prev_cost = cost;
    
    reset_ants();
    
    return cost;
}

void ACO::rollback_evaporation(int tick, float value) {
    for (int t = 0; t <= tick; ++t) {
        for (list<base_ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
            dynamic_cast<ant*>(*it)->roll_back(t, value);
        }
    }
}

void ACO::evaporate_update_future_pheromones(int ticks) {
    vector<t_edge*> edges;
    float max = 0.0f;
    float current = 0.0f;
    for (int i = 0; i < g->get_num_nodes(); i++) {
        edges = (*g)[i]->get_edges();
        for (int j = 0; j < edges.size(); j++) {
            for (int k = ticks; k >= 0; k--) {
                edges[j]->evaporate(k, conf.getRho());
                
                current = edges[j]->get_pheromone(k).current;
                if (current > max) {
                    max = current;
                    edges[j]->update_future_pheromone(k, max);
                }
            }
        }
    }
}

double ACO::cost_evaluation(int max_duration, list<base_ant*> base_ants) {
    double total_cost = 0;
    
    num_ants = base_ants.size();
    string** print_route = new string*[num_ants];
    for (int i = 0; i < num_ants; i++)
        print_route[i] = new string[max_duration+1];

    for (int tick = 0; tick <= max_duration; tick++) {
        map< iPair, int > map_ant_count;
        int ant_index = -1;
        for (list<base_ant*>::iterator it = base_ants.begin(); it != base_ants.end(); ++it) {
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
                
            }
        }
        // cost calculation per tick
        total_cost += cost_per_tick(map_ant_count);
    }
    
    if (conf.DEBUG()) {
        log_results(max_duration, total_cost, print_route);
    }
    
    for (int i = 0; i < num_ants; i++)
        delete [] print_route[i];
    delete [] print_route;
    return total_cost;
}

void ACO::log_results(int tick, int cost, string** print_route) {
    
    
    cout << "\n" << "ITERATION NUMBER " << num_iters << "\n";
    cout<<setw(20);
    list<string> r = d_map->get_manifest_routes();
    int ant_num = 0;
    for (list<string>::iterator it = r.begin(); it != r.end(); ++it) {
        vector<string> path = split((*it), ' ');
        cout << "Ant" << ant_num << " "<< path.front() << "->" << path.back() << setw(5);;
        ant_num++;
    }
    
    for (int i = 0; i <= tick; i++) {
        cout<< "\n" << "tick" << i << setw(16);
        for (int j = 0; j < num_ants; j++) {
            cout << print_route[j][i] << setw(16);
        }
    }
    cout << "Cost: " << cost << "\n";

}

void ACO::log_rollback(int node_id) {

    cout << "ITERATION NUMBER " << num_iters << "\n";
    cout << "ROLLBACK at: " << node_id << endl;

}

double ACO::cost_per_tick(map< iPair, int > map_ant_count) {
    map< iPair, int >::iterator itm;
    double                      cost_per_tick = 0;
    
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

double ACO::cost_based_num_ants(int num_of_ants) {
    int     num_at_middle = num_of_ants - 2;
    double  cost_for_middle = 0;
    double  cost_for_last = 0;
    double  avg_cost = 1;
    
    if (num_of_ants == 1)
        return avg_cost;

    if (num_at_middle > 0)
        cost_for_middle = num_at_middle * (avg_prcnt_fuel_saving_by_middle/100);

    cost_for_last = (avg_prcnt_fuel_saving_by_last/100);

    avg_cost = 1 - ((cost_for_middle + cost_for_last)/num_of_ants);

    return avg_cost;
}
