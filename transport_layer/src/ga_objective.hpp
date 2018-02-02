//
//  ga_objective.hpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-02-02.
//

#ifndef ga_objective_hpp
#define ga_objective_hpp

#include <stdio.h>

#include "../lib/ga_lib/src/Galgo.hpp"

#include "ACO.hpp"
#include "Dijkstra.hpp"
#include "cost_function.hpp"
#include "heuristic_selector.hpp"

#include "../../models/config.hpp"
#include "../../models/map_data.hpp"
#include "../../models/manifest.hpp"
#include "../../models/graph.hpp"

template <typename T>
class ga_objective {
public:
    static std::vector<T> Objective(const std::vector<T>& x);
    static map_data*& map();
    static manifest*& manifest_d();
    static Dijkstra*& dijkstra();
    static int& num_iters();

};

inline map_data*& ga_objective::map() {
    static map_data* m = new map_data();
    return m;
}

inline manifest*& ga_objective::manifest_d() {
    static manifest* m = new manifest();
    return m;
}

inline Dijkstra*& ga_objective::dijkstra() {
    static Dijkstra* d = new Dijkstra();
    return d;
}

inline int& ga_objective::num_iters() {
    static int i = 0;
    return i;
}

#endif /* ga_objective_hpp */
