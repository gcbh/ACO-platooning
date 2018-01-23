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

namespace fydp {
    class Segment {

    public:
        std::string type;
        int start_node;
        int end_node;
        int time;

    private:
    };

//    void to_json(json& j, const Segment& s) {
//        j = json{
//            {"type", s.type},
//            {"start_node", s.start_node},
//            {"end_node", s.end_node},
//            {"time", s.time},
//        };
//    }
//
//    void from_json(const json& j, Segment& s) {
//        s.type = j.at("type").get<std::string>();
//        s.start_node = j.at("start_node").get<int>();
//        s.end_node = j.at("end_node").get<int>();
//        s.time = j.at("time").get<int>();
//    }
}

#endif /* Segment_hpp */
