//
//  Segment.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2018-01-22.
//  Copyright © 2018 FYDP. All rights reserved.
//

#include "Segment.hpp"
#include "SimScene.hpp"

Segment::Segment(json j) {
    type = j.at("type").get<std::string>();
    start_node = SimScene::city_map[j.at("start_node").get<int>()];
    end_node = SimScene::city_map[j.at("end_node").get<int>()];
    time = j.at("time").get<double>();
    max_wait = j.at("max_wait").get<double>();
    platoon_members = j.at("platoon_members").get<std::vector<int>>();
}

bool Segment::isEqual(Segment* segment) {
    return (
        type == segment->type &&
        start_node == segment->start_node &&
        end_node == segment->end_node &&
        time == segment->time
    );
}
