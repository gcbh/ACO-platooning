//
//  TruckNode.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2018-01-22.
//  Copyright © 2018 FYDP. All rights reserved.
//

#ifndef TruckNode_hpp
#define TruckNode_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "Segment.hpp"
#include "QuadNode.hpp"
#include "json.hpp"
#include "State.hpp"

// for convenience
using json = nlohmann::json;

enum class TruckState : int {
    Waiting,
    Driving,
    Complete
};

class TruckNode : public QuadNode
{
public:

    TruckNode() : QuadNode() {}
    TruckNode(json json);

    void postsetup();
    void preinput(InputState* is);
    void update(UpdateState* us);
    void prerender(RenderState* rs);
    void render(RenderState* rs);

    void drawTruck(RenderState* rs);
    int m_id;
    TruckType m_type;
    TruckState m_state;
    std::vector<Segment*> m_schedule;

private:
    Segment* current_segment;
};

#endif /* TruckNode_hpp */
