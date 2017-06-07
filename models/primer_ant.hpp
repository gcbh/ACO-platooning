//
//  primer_ant.hpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#ifndef primer_ant_hpp
#define primer_ant_hpp

#include <stdio.h>

class primer_ant : base_ant {
public:
    //TODO: constructor must take in destination node
    primer_ant(std::queue<int> i_nodes);
};

#endif /* primer_ant_hpp */
