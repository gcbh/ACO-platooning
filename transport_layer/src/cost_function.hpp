//
//  cost_function.hpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-01-15.
//

#ifndef cost_function_hpp
#define cost_function_hpp

#include <stdio.h>
#include <list>

#include "../../models/primer_ant.hpp"
#include "../../models/ant.hpp"
#include "../../models/graph.hpp"

typedef pair<int, int> iPair;

class cost_function {
public:
    cost_function();
    double evaluate(map<path, int> segments);
private:
    double cost_per_tick(graph g, map< iPair, int > map_ant_count);
    double cost_based_num_ants(int num_of_ants);
};

#endif /* cost_function_hpp */
