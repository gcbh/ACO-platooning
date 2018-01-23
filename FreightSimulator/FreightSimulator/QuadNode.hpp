//
//  QuadNode.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-18.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef QuadNode_hpp
#define QuadNode_hpp

#include <stdio.h>
#include <string>
#include "SceneNode.hpp"

class QuadNode : public SceneNode
{
public:

    QuadNode() : SceneNode() {}

    void setup();
    void render(RenderState* rs);

private:
    static GLuint vbo, vao, program;
};

#endif /* QuadNode_hpp */
