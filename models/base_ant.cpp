//
//  base_ant.cpp
//  models
//
//  Created by Geoffrey Heath on 2017-06-07.
//  Copyright © 2017. All rights reserved.
//

#include "base_ant.hpp"

using namespace std;

base_ant::base_ant() {
}

base_ant::~base_ant() {
    delete ordered_path;
}

void base_ant::next_node(int time) {    }
