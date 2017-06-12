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

class SceneNode
{
public:
    SceneNode();
    ~SceneNode();

private:
    void input();
    void update();
    void render();
};

#endif /* SceneNode_hpp */
