//
//  SceneNode.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-06-12.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "SceneNode.hpp"

SceneNode::SceneNode() {
    _setup();
}

SceneNode::~SceneNode() { }

void SceneNode::_setup() {
    fprintf(stdout, "Node Private Setup.\n");

    m_model_matrix = glm::mat4(1.0);
    willRender = true;

    setup();
}

void SceneNode::_input() {
    //User input
    input();

    //Input children
    std::vector<SceneNode*>::iterator itr = child_nodes.begin();
    while( itr != child_nodes.end()) {
        SceneNode* node = *itr;
        node->_input();
        itr++;
    }
}

void SceneNode::_update(UpdateState us) {
    //Update self

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
        //glUniformMatrix4fv(m_mvp_id, 1, GL_FALSE, &rs.mvp[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        fprintf(stdout, "Node Private Render %d %d %d\n", m_program, m_vertex_array, m_mvp_id);
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
    child_nodes.push_back(node);
}
