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

class ant : base_ant {
public:
    // TODO: must pass in destination node
    
    // TODO: must decide if queue or set is best option
    ant(std::queue<int> i_nodes);
private:
    // TODO: must hold destination nodes
    // Node dest;
    int counter;
};
#endif /* ant_hpp */
