//
//  base_ant.hpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#ifndef base_ant_hpp
#define base_ant_hpp

#include <stdio.h>

class base_ant {
public:
    base_ant(std::queue<int> i_nodes);
    void traverse_path();
    
protected:
    std::queue<int> nodes;
    
};

#endif /* base_ant_hpp */
