//
//  EdgeNode.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-17.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef EdgeNode_hpp
#define EdgeNode_hpp

#include <stdio.h>
#include <string>
#include "SceneNode.hpp"

class EdgeNode : public SceneNode
{
public:

    EdgeNode() : SceneNode() {}

    void setup();
    void input();
    void update(UpdateState us);
    void render(RenderState rs);

    std::pair<int,int> m_id;
    float m_weight;
    float m_static_heat;

private:
};

#endif /* EdgeNode_hpp */
