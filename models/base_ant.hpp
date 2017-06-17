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
    base_ant();
    void next_node(int time);
    
protected:
    std::queue<int> ordered_path;
    t_node *current;
};

#endif /* base_ant_hpp */
