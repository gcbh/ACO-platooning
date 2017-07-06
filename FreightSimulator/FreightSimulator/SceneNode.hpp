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
#include <vector>
#include "glm.hpp"
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include "State.hpp"

class SceneNode
{
public:
    SceneNode();
    ~SceneNode();

    void setup() {};
    void input() {};
    void update(UpdateState us) {};
    void render(RenderState rs) {};

    void _setup();
    void _input();
    void _update(UpdateState us);
    void _render(RenderState rs);

    void addChildNode(SceneNode* node);

    glm::mat4 m_model_matrix;
    GLuint m_program, m_vertex_array, m_mvp_id;
    bool willRender;

private:

    std::vector<SceneNode*> child_nodes;
};

#endif /* SceneNode_hpp */
