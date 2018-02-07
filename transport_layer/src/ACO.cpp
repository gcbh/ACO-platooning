//
//  ACO.cpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#include "ACO.hpp"

ACO::ACO(graph *i_g, manifest i_manifest, config i_conf, heuristic_selector* i_sel, cost_function* i_j, logger standard, logger cost, logger debug) {
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
    num_iters = 0;
    lowest_cost = DBL_MAX;
    std_out = standard;
    cost_out = cost;
    debug_log = debug;
}

ACO::~ACO() {
    for (int i = 0; i < ants.size(); ++i) {
        delete output[i];
        delete lowest_cost_route[i];
    }
    delete output;
    delete lowest_cost_route;
}

void ACO:: init(Dijkstra *dijkstra) {
    d_map = dijkstra;

    set_prime_ant();

//    cout << setw(50) << "***BEGINNING ANT COLONY OPTIMIZATION***\n\n";
//    cout << setw(50) << "------------------------------------------------------------\n\n";
//    cout << setw(50) << "ANT PATHS" << endl;
    std_out.log(INFO, "Start your engines");
    reset_ants();
}

void ACO:: set_prime_ant() {
    int               tick = -1;
    vector<string>::  iterator it;
    bool              endIteration;
    double            cost = 0;
    
    vector<string>    manifest_route = d_map->get_manifest_routes();
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
    vector<string> manifest_route = d_map->get_manifest_routes();
    for (vector<string>::iterator it = manifest_route.begin(); it != manifest_route.end(); it++) {
        string         route = *it;
        vector<string> nodes = split(route, ' ');
        int            src = stoi(nodes[0]);
        int            dest = stoi(nodes[nodes.size() - 1]);
        t_node*        start_from = (*g)[src];
        ant *a = new ant(start_from, dest, conf.getDelta(), sel);
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
                int start_id = p.first->get_id();
        
                string start = to_string(start_id);
                string dest;
                
                if (p.second) {
                    int dest_id = p.second->get_dest()->get_id();
                    dest = to_string(dest_id);
                    
                    rollback_ant(start_id, dest_id, (*it), tick);
                    
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
    cost_out.log(INFO, to_string(cost));
    debug_log.log(DEBUG, "Cost: " + to_string(cost));
    return cost;
}

void ACO::rollback_ant(int start, int dest, base_ant* ant, int tick) {
    if (!ant->get_did_reach_destination()) {
        t_edge* e = (*g)[start]->get_edge(dest);
        e->update_pheromone(tick, -(1.5 * conf.getDelta()));
    }
}

void ACO::evaporation(unordered_set<position, position_hash> traversed, double mag, int ticks) {
    vector<t_edge*> edges;
    unordered_set<int> nodes = g->get_nodes();
    for (unordered_set<int>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
        edges = (*g)[(*it)]->get_edges();
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
    bool reached_dest = true;
    for (list<base_ant*>::iterator it = ants.begin(); it != ants.end(); ++it) {
        if (!(*it)->has_reached_destination()) {
            cost += 100000.0;
            reached_dest = false;
        } else {
            reached_dest = true;
        }
        // replay_route() in evaluation will reset current which changes the result of has_reached_destination().
        // Need to hold this property for rollback.
        (*it)->set_did_reach_destination(reached_dest);
    }
    return cost;
}

void ACO::init_log() {
    vector<string> manifest_route = d_map->get_manifest_routes();
    int ant_num = 0;
    string output = "\n ITERATION NUMBER" + to_string(num_iters) + "\n" + space(20);

    for (vector<string>::iterator it = manifest_route.begin(); it != manifest_route.end(); ++it) {
        vector<string> path = split((*it), ' ');
        output += "Ant" + to_string(ant_num) + " " + path.front() + "->" + path.back() + space(10);
        ++ant_num;
    }
    output += "\n";
    debug_log.log(DEBUG, output);
}

void ACO::log_tick(int tick, vector<string> segments) {
    string output = "tick" + to_string(tick) + space(16);
    for (vector<string>::iterator it = segments.begin(); it != segments.end(); ++it) {
        output += (*it) + space(16);
    }
    debug_log.log(DEBUG, output);
}

void ACO::save_lowest_cost_route() {

    for(int ant = 0; ant < ants.size(); ant++) {
        delete lowest_cost_route[ant];
        vector<string>* ant_route = output[ant];
        lowest_cost_route[ant] = new vector<string> (*ant_route);
    }

}

vector<string>** ACO::result() { return lowest_cost_route; }
