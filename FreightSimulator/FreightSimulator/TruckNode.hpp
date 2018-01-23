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

// for convenience
using json = nlohmann::json;

class TruckNode : public QuadNode
{
public:

    TruckNode() : QuadNode() {}
//    TruckNode(json json);

    void postsetup();
    void preinput(InputState is);
    void preupdate(UpdateState us);
    void prerender(RenderState rs);

    int m_id;
    std::vector<fydp::Segment*> m_schedule;

private:
};

#endif /* TruckNode_hpp */
