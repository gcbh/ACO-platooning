//
//  base_ant.hpp
//  models
//
//  Created by Aditi Lath on 2017-07-17.
//  Copyright © 2017 University of Waterloo. All rights reserved.
//

#ifndef base_ant_hpp
#define base_ant_hpp

#include <stdio.h>
#include <queue>

#include "t_node.hpp"

using namespace std;

typedef pair<int, int> iPair;

class base_ant {
public:
    base_ant();
    virtual         ~base_ant();
    void            init_cost();
    virtual void    next_node(int time);
    bool            has_reached_destination();
    iPair           cost_node(int time);
    queue<t_node*>  get_ordered_path();
protected:
    int             counter, dest;
    t_node*         current;
    queue<t_node*>  ordered_path;
    
};

#endif /* base_ant_hpp */
