//
//  ACO_new.hpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#ifndef ACO_new_hpp
#define ACO_new_hpp

#include "../aco/include/Randoms.h"
#include <map>
#include "../../models/t_node.hpp"
#include "../../models/graph.hpp"
#include "../../models/t_edge.hpp"

class ACO_new {
public:
    ACO_new (graph *i_g, int i_num_iterations);
    virtual ~ACO_new ();
private:
    graph *g;
    int num_iterations;
    void iteration();
    void delta_pheromone(int time, t_edge *edge);
    void evaporation();
};

#endif /* ACO_new_hpp */
