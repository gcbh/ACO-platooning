//
//  Segment.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2018-01-22.
//  Copyright © 2018 FYDP. All rights reserved.
//

#include "Segment.hpp"

Segment::Segment(json j) {
    type = j.at("type").get<std::string>();
    start_node = j.at("start_node").get<int>();
    end_node = j.at("end_node").get<int>();
    time = j.at("time").get<double>();
    max_wait = j.at("max_wait").get<int>();
    platoon_members = j.at("platoon_members").get<std::vector<int>>();
}
