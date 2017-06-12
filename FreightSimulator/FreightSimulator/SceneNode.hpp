//
//  SceneNode.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-06-12.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef SceneNode_hpp
#define SceneNode_hpp

#include <stdio.h>
#include "linmath.h"

class SceneNode
{
public:
    SceneNode();
    ~SceneNode();

    void setup();
    void input();
    void update();
    void render();

private:
    mat4x4 m_model;
};

#endif /* SceneNode_hpp */
