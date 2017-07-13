//
//  ant.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#include "ant.hpp"

using namespace std;

ant::ant(t_node *first, int i_dest, float i_alpha, float i_beta) {
    dest = i_dest;
    counter = 0;
    current = first;
    ordered_path = new queue<int>();
    past_nodes = new set<int>();
    //r = new Randoms(21);
    alpha = i_alpha;
    beta = i_beta;
}

ant::~ant() {
    delete past_nodes;
}

void ant::next_node(int time) {
    double total = 0;
    double total_prob = 0;
    int best_wait = 0;
    for (int i = 0; i < current->edge_number(); i++) {
        t_edge* e = (current*)[i];
        if (past_nodes->find(e->get_dest()->get_id()) == past_nodes->end()) {
            phermone p = e->get_phermone(time);
            total += (double)p.current * alpha;
            if (p.future > best_wait) {
                best_wait = p.future;
            }
        }
    }
    
    total += (double)best_wait * alpha;
    
    double prob = r.uniforme();
    for (int i = 0; i < current->edge_number(); i++) {
        t_edge* e = (current*)[i];
        if (past_nodes->find(e->get_dest()->get_id()) == past_nodes->end()) {
            phermone p = e->get_phermone(time);
            total_prob += alpha * p.current / total;
            if (total_prob < prob) {
                int d = e->get_dest()->get_id();
                past_nodes->insert(d);
                ordered_path->push(d);
                current = e->get_dest();
                return;
            }
        }
    }
    
}

bool ant::hasReachedDestination() {
    return current->get_id() == dest;
}

queue<int> ant::get_ordered_path() {
    return *ordered_path;
}