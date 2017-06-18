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
#include <queue>

#include "t_node.hpp"

using namespace std;

class base_ant {
public:
    base_ant();
    virtual ~base_ant();
    virtual void next_node(int time);
    
protected:
    queue<int> *ordered_path;
    t_node *current;
};

#endif /* base_ant_hpp */
