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
    m_position = glm::vec3(0.0);
    m_velocity = glm::vec3(0.0);

    m_program = -1;
    m_vertex_array = -1;
    m_mvp_id = -1;
    willRender = true;

    setup();
}

void SceneNode::_input(InputState is) {
    //User input
    input(is);

    //Input children
    std::vector<SceneNode*>::iterator itr = child_nodes.begin();
    while( itr != child_nodes.end()) {
        SceneNode* node = *itr;
        node->_input(is);
        itr++;
    }
}

void SceneNode::_update(UpdateState us) {
    //Update self
    m_position = m_position + (m_velocity * (float)us.deltaTime);

    glm::mat4x4 translate = glm::translate(glm::mat4(1.0), m_position);
    glm::mat4x4 scale = glm::scale(glm::mat4(1.0), m_scale);
    m_model_matrix = translate;

    //User update
    update(us);

    //Update children
    std::vector<SceneNode*>::iterator itr = child_nodes.begin();
    while( itr != child_nodes.end()) {
        SceneNode* node = *itr;
        node->_update(us);
        itr++;
    }
}

void SceneNode::_render(RenderState rs) {

    //Perform mvp update
    rs.mvp = rs.mvp * m_model_matrix;

    if (willRender) {
        //User render
        render(rs);

        //Render self
        glUseProgram(m_program);
        glBindVertexArray(m_vertex_array);
        glUniformMatrix4fv(m_mvp_id, 1, GL_FALSE, &rs.mvp[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    //Render children
    std::vector<SceneNode*>::iterator itr = child_nodes.begin();
    while( itr != child_nodes.end()) {
        SceneNode* node = *itr;
        node->_render(rs);
        itr++;
    }
}

void SceneNode::addChildNode(SceneNode* node) {
    node->_setup();
    child_nodes.push_back(node);
}
