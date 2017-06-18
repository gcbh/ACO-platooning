//
//  ant.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#include "ant.hpp"

using namespace std;

ant::ant(t_node *first, int i_dest) {
    dest = i_dest;
    counter = 0;
    current = first;
    ordered_path = new queue<int>();
    past_nodes = new set<int>();
}

ant::~ant() {
    delete past_nodes;
}

void ant::next_node(int time) {
    
}
