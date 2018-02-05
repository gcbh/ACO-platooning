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
        
        time_t seed = (long)time(nullptr);
        
        logger std_out;
        logger cost_out("../logs/" + to_string(seed) + "-cost.out", false);
        logger debug_log("../logs/" + to_string(seed) + "-debug.log", false);
        
        config c("", "", x[0], x[1], x[2], x[3], x[4], x[5], false, 100);
        
        heuristic_selector* sel = new heuristic_selector(c.getAlpha(), c.getBeta(), c.getPhi(), seed, ga_objective<T>::dijkstra());
        
        graph *g = new graph();
        g->construct_graph((*ga_objective<T>::map()));
        
        ACO *aco = new ACO(g, (*ga_objective<T>::manifest_d()), c, sel, ga_objective<T>::j(), std_out, cost_out, debug_log);
        aco->init(ga_objective<T>::dijkstra());
        
        T full_sum = 0.0;
        
        try {
            for(int i = 1; i <= ga_objective<T>::num_iters(); i++) {
                int cost = aco->iteration();
                if (cost < 0) continue;
                else full_sum += cost;
            }
            
            // generate final output
//            write_final_output(aco, g, manifest_map.size());
            
            delete g;
            delete sel;
            delete aco;
            
        } catch (const exception &e) {
            cout << e.what() << endl;
        }
        return {full_sum / ga_objective<T>::num_iters()};;
    }
    static map_data*& map();
    static manifest*& manifest_d();
    static Dijkstra*& dijkstra();
    static cost_function*& j() {
        static cost_function* cf = new cost_function();
        return cf;
    };
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
