//
//  cost_function.cpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-01-15.
//

#include "cost_function.hpp"

const float avg_prcnt_fuel_saving_by_middle = 4.1;
const float avg_prcnt_fuel_saving_by_last = 6.1;

cost_function::cost_function() {    }

double cost_function::evaluate(graph g, list<base_ant*> base_ants, int max_duration) {
    double total_cost = 0;
    
    int num_ants = base_ants.size();
    
    for (int tick = 0; tick <= max_duration; tick++) {
        map< iPair, int > map_ant_count;
        int ant_index = -1;
        for (list<base_ant*>::iterator it = base_ants.begin(); it != base_ants.end(); ++it) {
            ant_index++;
            if (!(*it)->has_reached_destination()) {
                iPair nodes_pair = (*it)->cost_node(tick);
                int pair_elem1 = get<0> (nodes_pair);
                int pair_elem2 = get<1> (nodes_pair);
                
                int ant_count = 0;
                if (map_ant_count.count(nodes_pair))
                    int ant_count = map_ant_count.find(nodes_pair)->second;
                
                map_ant_count.insert(make_pair(nodes_pair, ant_count+1));
                
            }
        }
        // cost calculation per tick
        total_cost += cost_per_tick(g, map_ant_count);
    }
    
    return total_cost;
}

double cost_function::cost_per_tick(graph g, map< iPair, int > map_ant_count) {
    map< iPair, int >::iterator itm;
    double                      cost_per_tick = 0;
    
    for (itm = map_ant_count.begin(); itm != map_ant_count.end(); ++itm) {
        int num_of_ants = itm->second;
        
        iPair nodes_pair = itm->first;
        t_node* src = g[get<0> (nodes_pair)];
        t_node* dest = g[get<1> (nodes_pair)];
        t_edge* edg = src->get_edge(dest->get_id());
        int edge_cost = edg->get_distance();
        cost_per_tick += (edge_cost*cost_based_num_ants(num_of_ants));
    }
    return cost_per_tick;
}

double cost_function::cost_based_num_ants(int num_of_ants) {
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
