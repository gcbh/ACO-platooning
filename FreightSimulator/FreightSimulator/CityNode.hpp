//
//  CityNode.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-06.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef CityNode_hpp
#define CityNode_hpp

#include <stdio.h>
#include <string>
#include "SceneNode.hpp"

class CityNode : public SceneNode
{
public:

    CityNode() : SceneNode() {}

    void setup();
    void input();
    void update(double deltaTime);
    void render(RenderState rs);

    int m_id;
    std::string m_name;
    
private:
};

#endif /* CityNode_hpp */
