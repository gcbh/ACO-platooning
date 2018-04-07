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

    void postsetup();
    void preinput(InputState* is);
    void preupdate(UpdateState* us);
    void prerender(RenderState* rs);
    void render(RenderState* rs);

    int m_id;
    std::pair<glm::vec4,glm::vec4> m_position_pair;
    float m_weight;
    float m_static_heat;

private:
    ImVec4 generate_static_color(RenderState* rs);
    ImVec4 generate_dynamic_color(RenderState* rs);
};

#endif /* EdgeNode_hpp */
