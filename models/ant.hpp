//
//  ant.hpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#ifndef ant_hpp
#define ant_hpp

#include <stdio.h>
#include <queue>
#include <set>

using
class ant : base_ant {
public:
    // TODO: must pass in destination node
    
    // TODO: must decide if queue or set is best option
    ant(t_node *first, int i_dest);
private:
    t_node *current;
    int dest, counter;
    
    set<int> past_nodes;
};
#endif /* ant_hpp */
