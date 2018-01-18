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
typedef pair<t_node*, t_edge*> path;

class base_ant {
public:
    base_ant();
    virtual         ~base_ant();
    virtual path    next_node(int time);
    bool            has_reached_destination();
    bool            in_transit();
    virtual bool    has_concluded();
    path            replay_route();
    queue<t_node*>  get_ordered_path();
protected:
    int                      counter, dest;
    t_node*                  current;
    queue<t_node*>           ordered_path;
    
};

#endif /* base_ant_hpp */
