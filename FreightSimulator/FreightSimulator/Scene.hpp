//
//  Scene.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-06-12.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include "SceneNode.hpp"
#include "State.hpp"
#include "glm.hpp"

class Scene
{
public:
    Scene();
    ~Scene();

    virtual void setup() = 0;
    virtual void input() = 0;
    virtual void update(double deltaTime) = 0;
    virtual void render(RenderState rs) = 0;

    SceneNode* m_root_node;
    glm::mat4 m_projection_matrix;
    glm::mat4 m_view_matrix;

    void _setup();
    void _input();
    void _update(double deltaTime);
    void _render(RenderState rs);

private:
    /*void _setup();
    void _input();
    void _update(double deltaTime);
    void _render(RenderState rs);*/
};

#endif /* Scene_hpp */
