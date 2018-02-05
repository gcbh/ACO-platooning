//
//  ga_objective.hpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-02-02.
//

#ifndef ga_objective_hpp
#define ga_objective_hpp

#include "ACO.hpp"
#include "Dijkstra.hpp"
#include "cost_function.hpp"
#include "heuristic_selector.hpp"

#include "config.hpp"
#include "map_data.hpp"
#include "manifest.hpp"
#include "graph.hpp"

template <typename T>
class ga_objective {
public:
    static std::vector<T> Objective(const std::vector<T>& x) {
        config c("", "", x[0], x[1], x[2], x[3], x[4], x[5], false, 100);
        time_t seed = (long)time(nullptr);
        heuristic_selector* sel = new heuristic_selector(c.getAlpha(), c.getBeta(), c.getPhi(), seed, ga_objective<T>::dijkstra());
    
        cost_function* cost = new cost_function();
        return x;
    }
    static map_data*& map();
    static manifest*& manifest_d();
    static Dijkstra*& dijkstra();
    static int& num_iters();

};

template <typename T>
inline map_data*& ga_objective<T>::map() {
    static map_data* m = new map_data();
    return m;
};

template <typename T>
inline manifest*& ga_objective<T>::manifest_d() {
    static manifest* m = new manifest();
    return m;
};

template <typename T>
inline Dijkstra*& ga_objective<T>::dijkstra() {
    static Dijkstra* d = new Dijkstra();
    return d;
};

template <typename T>
inline int& ga_objective<T>::num_iters() {
    static int i = 0;
    return i;
};



#endif /* ga_objective_hpp */
