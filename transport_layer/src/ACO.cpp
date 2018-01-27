//
//  ACO.cpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#include "ACO.hpp"

ACO::ACO(graph *i_g, manifest i_manifest, config i_conf, heuristic_selector* i_sel, cost_function* i_j) {
    g = i_g;
    conf = i_conf;
    sel = i_sel;
    j = i_j;
    manifest_data = i_manifest;
    output = new vector<string>*[i_manifest.size()];
    lowest_cost_route = new vector<string>*[i_manifest.size()];
    for (int i = 0; i < i_manifest.size(); ++i) {
        output[i] = new vector<string>();
        lowest_cost_route[i] = new vector<string>();
    }
    RESULT_LOG_PATH = "../results.log";
    num_iters = 0;
    result_log.open(RESULT_LOG_PATH);
    lowest_cost = DBL_MAX;
    freopen(RESULT_LOG_PATH.c_str(), "w", stdout);
}

ACO::~ACO() {
    result_log.close();
    fclose(stdout);
    for (int i = 0; i < ants.size(); ++i) {
        delete output[i];
        delete lowest_cost_route[i];
    }
    delete output;
    delete lowest_cost_route;
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
            if (!(*it)->has_concluded()) {
                (*it)->next_node(tick);   
                endIteration = false;   
            }
        }
    } while(!endIteration);
    
    cout << setw(50) << "*** DIJKSTRA COST ***\n\n";
    cout << setw(50) << "------------------------------------------------------------\n\n";
    
    cost = evaluation(tick);
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
    bool    endIteration;
    unordered_set<position, position_hash> traversed;
    for (int i = 0; i < ants.size(); ++i) {
        delete output[i];
        output[i] = new vector<string>();
    }
    
    num_iters++;
    
    do {
        endIteration = true;
        tick++;
        for (list<base_ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
            //if ant has not reached destination call nextnode
            if (!(*it)->has_concluded()) {
                path p = (*it)->next_node(tick);
                if (p.second) {
                    position pos = { .time = tick, .edge_id = p.second->get_id() };
                    traversed.insert(pos);
                }
                endIteration = false;   
            }
        }
    } while(!endIteration);

    cost += evaluation(tick);
    
    double evap_mag = conf.getLambda();
    
    if (cost < lowest_cost) {
        evap_mag = evap_mag * (lowest_cost / cost);
        lowest_cost = cost;
        save_lowest_cost_route();
    } else if (cost > lowest_cost) {
        evap_mag *= -1.0 * cost / lowest_cost;
    } else {
        evap_mag = 0.0;
    }
    
    evaporation(traversed, evap_mag, tick);
    
    reset_ants();

    return cost;
}

double ACO::evaluation(int max_duration) {
    double cost = 0;
    
    if (conf.DEBUG()) init_log();
    
    cost += path_failure_penalty();
    
    for (int tick = 0; tick < max_duration; tick++) {
        map<path, int> segments;
        vector<string> actions;
        int ant_num = 0;
        
        for (list<base_ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
            path p = (*it)->replay_route();
            
            if (p.first) {
                string start = to_string(p.first->get_id());
                string dest;
                
                if (p.second) {
                    dest = to_string(p.second->get_dest()->get_id());
                    if (segments.find(p) == segments.end()) {
                        segments.insert(make_pair(p, 1));
                    } else {
                        segments[p] = segments[p] + 1;
                    }
                } else {
                    dest = start;
                }
                build_output(ant_num, start + "->" + dest, &actions);
            } else {
                if ((*it)->in_transit()) {
                    build_output(ant_num, "Transit", &actions);
                } else {
                    build_output(ant_num, "", &actions);
                }
            }
            
            ant_num++;
        }
        
        if (conf.DEBUG()) log_tick(tick, actions);
        
        cost += j->evaluate(segments);
    }
    
    log_cost(cost);
    
    return cost;
}

void ACO::evaporation(unordered_set<position, position_hash> traversed, double mag, int ticks) {
    vector<t_edge*> edges;
    for (int i = 0; i < g->get_num_nodes(); i++) {
        edges = (*g)[i]->get_edges();
        for (int j = 0; j < edges.size(); j++) {
            float max = 0.0f;
            float current = 0.0f;
            t_edge* e = edges[j];
            for (int k = ticks; k >= 0; k--) {
                position p = { .time = k, .edge_id = e->get_id() };
                
                if (traversed.find(p) != traversed.end()) {
                    e->update_pheromone(k, mag);
                }
                
                e->evaporate(k, conf.getRho());
                
                current = e->get_pheromone(k).current;
                if (current > max) {
                    max = current;
                    e->update_future_pheromone(k, max);
                }
            }
        }
    }
}

void ACO::build_output(int ant_num, string act, vector<string> *actions) {
    actions->push_back(act);
    output[ant_num]->push_back(act);
}

double ACO::path_failure_penalty() {
    double cost = 0.0;
    for (list<base_ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
        if (!(*it)->has_reached_destination()) {
            cost += 100000.0;
        }
    }
    return cost;
}

void ACO::init_log() {
    int ant_num = 0;
    list<string> r = d_map->get_manifest_routes();
    
    cout << "\n" << "ITERATION NUMBER " << num_iters << "\n";
    cout << setw(20);
    
    for (list<string>::iterator it = r.begin(); it != r.end(); ++it) {
        vector<string> path = split((*it), ' ');
        cout << "Ant" << ant_num <<
        " "<< path.front() << "->" << path.back() << setw(10);
        ++ant_num;
    }
    cout << endl;
}

void ACO::log_tick(int tick, vector<string> segments) {
    cout << "tick" << tick << setw(16);
    for (vector<string>::iterator it = segments.begin(); it != segments.end(); ++it) {
        cout << *it << setw(16);
    }
    cout << endl;
}

void ACO::log_cost(double cost) {
    cout << "Cost: " << cost << "\n";
}

void ACO::save_lowest_cost_route() {

    for(int ant = 0; ant < ants.size(); ant++) {
        delete lowest_cost_route[ant];
        vector<string>* ant_route = output[ant];
        lowest_cost_route[ant] = new vector<string> (*ant_route);
    }

}

vector<string>** ACO::result() { return lowest_cost_route; }
