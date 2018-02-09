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

double cost_function::evaluate(map<path, int> segments) {
    double j = 0;
    
    for (auto it = segments.begin(); it != segments.end(); ++it) {
        int n = it->second;
        path p = it->first;
        t_edge* e = get<1> (p);
        
        int edge_cost = e->get_distance();
        j += edge_cost * cost_based_num_ants(n);
    }
    return j;
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
