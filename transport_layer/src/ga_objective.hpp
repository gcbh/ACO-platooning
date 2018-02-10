//
//  ga_objective.hpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-02-02.
//

#ifndef ga_objective_hpp
#define ga_objective_hpp

#include <time.h>

#include "ACO.hpp"
#include "Dijkstra.hpp"
#include "cost_function.hpp"
#include "heuristic_selector.hpp"
//#include "transport_exception.hpp"

#include "config.hpp"
#include "map_data.hpp"
#include "manifest.hpp"
#include "graph.hpp"

template <typename T>
class ga_objective {
public:
    static std::vector<T> Objective(const std::vector<T>& x) {
        
        int num_avg = 10;
        logger std_out;
        
        config c = build_config(x, std_out);
        
        if (c.getPhi() <= 2*(c.getAlpha()+c.getBeta())) {
            return {-1000000};
        }
        
        ACO* aco = build_ACO(c, std_out);
        aco->init(dijkstra());
        
        T full_sum = 0.0;
        
        try {
            for(int i = 1; i <= num_iters(); i++) {
                int cost = aco->iteration();
                if (cost < 0) continue;
                else if(num_iters() - i < num_avg) full_sum += cost;
            }
            
            delete aco;
        } catch (const exception &e) {
            cout << e.what() << endl;
        }
        return {-1 * full_sum / num_avg};
    };
    
    static cost_function *& cost_f(){
        static cost_function* cf = new cost_function();
        return cf;
    };
    
    static map_data*& map();
    static manifest*& manifest_d();
    static Dijkstra*& dijkstra();
    static int& num_iters();
    
private:
    static config build_config(std::vector<T> x, logger std_out){
        float alpha = (float)x[0];
        float primer = (float)x[1];
        float beta = (float)x[2];
        float delta = (float)x[3];
        float lambda = (float)x[4];
        float phi = (float)x[5];
        float rho = (float)x[6];
        
        config c("", "", "", alpha, primer, beta, delta, lambda, phi, rho, false, 100);
        
        std_out.log(INFO, "a: " + to_string(alpha) + " primer: " + to_string(primer) + " b: " + to_string(beta) + " d: " + to_string(delta) + " l: " + to_string(lambda) + " p: " + to_string(phi) + " r: " + to_string(rho));
        
        return c;
    };
    static ACO* build_ACO(config c, logger std_out) {
        time_t seed = (long)time(nullptr);
        
        logger cost_out("../logs/" + to_string(seed) + "-cost.out", false);
        logger debug_log("../logs/" + to_string(seed) + "-debug.log", false);
        
        heuristic_selector* sel = new heuristic_selector(c.getAlpha(), c.getBeta(), c.getPhi(), seed, dijkstra());
        
        graph *g = new graph();
        g->construct_graph((*map()));
        
        ACO *aco = new ACO(g, (*manifest_d()), c, sel, cost_f(), std_out, cost_out, debug_log);
        
        return aco;
    };
    
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
