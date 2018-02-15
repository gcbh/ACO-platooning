//
//  SceneNode.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-06-12.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "SceneNode.hpp"

SceneNode::SceneNode() { }

SceneNode::~SceneNode() { }

void SceneNode::_setup() {

    m_model_matrix = glm::mat4(1.0);
    m_rotation = 0.0f;
    m_position = glm::vec3(0.0);
    m_scale = glm::vec3(1.0f);
    m_velocity = glm::vec3(0.0);

    m_program = -1;
    m_vertex_array = -1;
    m_mvp_id = -1;
    willRender = true;

    presetup();
    setup();
    postsetup();
}

void SceneNode::_input(InputState* is) {

    preinput(is);
    input(is);
    postinput(is);

    //Input children
    std::vector<SceneNode*>::iterator itr = child_nodes.begin();
    while( itr != child_nodes.end()) {
        SceneNode* node = *itr;
        node->_input(is);
        itr++;
    }
}

void SceneNode::_update(UpdateState* us) {
    //Update self
    m_position = m_position + (m_velocity * (float)us->deltaTime);

    m_model_matrix = glm::mat4(1.0);
    m_model_matrix = glm::translate(m_model_matrix, m_position);
    m_model_matrix = glm::rotate(m_model_matrix, (glm::mediump_float)m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    m_model_matrix = glm::scale(m_model_matrix, m_scale);

    preupdate(us);
    update(us);
    postupdate(us);

    //Update children
    std::vector<SceneNode*>::iterator itr = child_nodes.begin();
    while( itr != child_nodes.end()) {
        SceneNode* node = *itr;
        node->_update(us);
        itr++;
    }
}

void SceneNode::_render(RenderState* rs) {

    //Perform mvp update
    rs->mvp = rs->mvp * m_model_matrix;

    if (willRender) {
        prerender(rs);


        render(rs);
    }



    //Render children
    std::vector<SceneNode*>::iterator itr = child_nodes.begin();
    while( itr != child_nodes.end()) {
        SceneNode* node = *itr;
        node->_render(rs);
        itr++;
    }

    if (willRender) {
        postrender(rs);
    }

    //Reset MVP update
    rs->mvp = rs->mvp * glm::inverse(m_model_matrix);
}

ImVec2 SceneNode::getScreenSpace(RenderState* rs) {
    return getScreenSpace(rs, glm::vec4(0.0, 0.0, 0.0, 1.0));
}

ImVec2 SceneNode::getScreenSpace(RenderState* rs, glm::vec4 point) {
    glm::vec4 ss = rs->mvp * point;
    return ImVec2(rs->screen_size.x * (ss.x + 1.0)/2.0, rs->screen_size.y * (1.0 - ((ss.y + 1.0) / 2.0)));
}

//Once a node is added to another node, distrubute the new parent scene
void SceneNode::propagateParentScene(Scene* scene) {

    parentScene = scene;

    std::vector<SceneNode*>::iterator itr = child_nodes.begin();
    while( itr != child_nodes.end()) {
        SceneNode* node = *itr;
        node->propagateParentScene(scene);
        itr++;
    }
}

void SceneNode::addChildNode(SceneNode* node) {
    node->_setup();
    node->propagateParentScene(parentScene);
    child_nodes.push_back(node);
}

void SceneNode::insertChildNode(SceneNode* node) {
    node->_setup();
    node->propagateParentScene(parentScene);
    child_nodes.insert(child_nodes.begin(), node);
}
