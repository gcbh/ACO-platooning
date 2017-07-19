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
#include "Scene.hpp"

class Scene;

class SceneNode
{
public:
    SceneNode();
    ~SceneNode();

    //Public overrideable
    virtual void presetup() {};
    virtual void postsetup() {};
    virtual void preinput(InputState is) {};
    virtual void postinput(InputState is) {};
    virtual void preupdate(UpdateState us) {};
    virtual void postupdate(UpdateState us) {};
    virtual void prerender(RenderState rs) {};
    virtual void postrender(RenderState rs) {};

    //Internal final
    void _setup();
    void _input(InputState is);
    void _update(UpdateState us);
    void _render(RenderState rs);

    //Internal overrideable
    virtual void setup() {};
    virtual void input(InputState rs) {};
    virtual void update(UpdateState rs) {};
    virtual void render(RenderState rs) {};

    void addChildNode(SceneNode* node);

    Scene* parentScene;

    float m_rotation;
    glm::vec3 m_scale;
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::mat4 m_model_matrix;
    GLuint m_program, m_vertex_array, m_mvp_id;
    bool willRender;



private:
    void propagateParentScene(Scene* scene);
    std::vector<SceneNode*> child_nodes;
};

#endif /* SceneNode_hpp */
