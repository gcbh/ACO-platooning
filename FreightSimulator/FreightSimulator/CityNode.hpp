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
#include "QuadNode.hpp"

class CityNode : public QuadNode
{
public:

    CityNode() : QuadNode() {}

    void setup();
    void input();
    void update(UpdateState us);
    void prerender(RenderState rs);

    int m_id;
    std::string m_name;
    
private:
};

#endif /* CityNode_hpp */
