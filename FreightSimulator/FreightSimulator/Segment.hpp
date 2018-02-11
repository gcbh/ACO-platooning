//
//  Segment.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2018-01-22.
//  Copyright © 2018 FYDP. All rights reserved.
//

#ifndef Segment_hpp
#define Segment_hpp

#include <stdio.h>
#include "json.hpp"

// for convenience
using json = nlohmann::json;

class Segment {

public:

    Segment(json json);

    std::string type;
    int start_node;
    int end_node;
    double time;
    int max_wait;
    std::vector<int> platoon_members;

private:
};

#endif /* Segment_hpp */