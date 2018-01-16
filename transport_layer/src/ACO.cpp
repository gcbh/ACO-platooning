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

ACO::ACO(graph *i_g, manifest i_manifest, config i_conf, heuristic_selector* i_sel, cost_function* i_j) {
    g = i_g;
    conf = i_conf;
    sel = i_sel;
    j = i_j;
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
    
    cout << setw(50) << "*** DIJKSTRA COST ***\n\n";
    cout << setw(50) << "------------------------------------------------------------\n\n";
    
//    cost = evaluation(tick);
    
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
    unordered_set<int> traversed;
    num_iters++;
    
    do {
        endIteration = true;
        tick++;
        for (list<base_ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
            //if ant has not reached destination call nextnode
            if (!(*it)->has_concluded()) {
                path p = (*it)->next_node(tick);
                if (p.second) traversed.insert(p.second->get_id());
                endIteration = false;   
            }
        }
    } while(!endIteration);

    cost += evaluation(tick);
    
    double evap_mag = conf.getDelta();
    
    if (cost > prev_cost) {
        prev_cost = cost;
        evap_mag *= -1.0;
    }
    
    evaporation(traversed, evap_mag, tick);
    
    reset_ants();
    
    return cost;
}

double ACO::evaluation(int max_duration) {
    double cost = 0;
    
    if (conf.DEBUG()) init_log();
    
    for (int tick = 0; tick <= max_duration; tick++) {
        map<path, int> segments;
        list<string> actions;
        
        for (list<base_ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
            path p = (*it)->replay_route();
            
            if (p.first) {
                if (segments.find(p) == segments.end()) {
                    segments.insert(make_pair(p, 1));
                } else {
                    segments[p] = segments[p] + 1;
                }
                string start = to_string(p.first->get_id());
                string dest = to_string(p.second->get_dest()->get_id());
                actions.push_back(start + "->" + dest);
            } else {
                if (!(*it)->has_concluded()) {
                    actions.push_back("Transit");
                }
            }
        }
        
        if (conf.DEBUG()) log_tick(tick, actions);
        
        cost += j->evaluate(segments);
    }
    
    log_cost(cost);
    
    return cost;
}

void ACO::evaporation(unordered_set<int> traversed, double mag, int ticks) {
    vector<t_edge*> edges;
    float max = 0.0f;
    float current = 0.0f;
    for (int i = 0; i < g->get_num_nodes(); i++) {
        edges = (*g)[i]->get_edges();
        for (int j = 0; j < edges.size(); j++) {
            for (int k = ticks; k >= 0; k--) {
                t_edge* e = edges[j];
                double evap_mag = conf.getRho();
                if (traversed.find(e->get_id()) != traversed.end()) {
                    evap_mag += mag;
                }
                edges[j]->evaporate(k, evap_mag);
                
                current = edges[j]->get_pheromone(k).current;
                if (current > max) {
                    max = current;
                    edges[j]->update_future_pheromone(k, max);
                }
            }
        }
    }
}

void ACO::init_log() {
    int ant_num = 0;
    list<string> r = d_map->get_manifest_routes();
    
    cout << "\n" << "ITERATION NUMBER " << num_iters << "\n";
    cout << setw(20);
    
    for (list<string>::iterator it = r.begin(); it != r.end(); ++it) {
        vector<string> path = split((*it), ' ');
        cout << "Ant" << ant_num <<
        " "<< path.front() << "->" << path.back() << setw(5);
    }
    cout << endl;
}

void ACO::log_tick(int tick, list<string> segments) {
    cout << "tick" << tick << setw(16);
    for (list<string>::iterator it = segments.begin(); it != segments.end(); ++it) {
        cout << *it << setw(16);
    }
    cout << endl;
}

void ACO::log_cost(double cost) {
    cout << "Cost: " << cost << "\n";
}