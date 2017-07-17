//
//  EdgeNode.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-17.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "EdgeNode.hpp"
#include "SimScene.hpp"
#include "glm.hpp"
#include "matrix_transform.hpp"

void EdgeNode::setup() {
    m_scale = glm::vec3(0.5f, 0.5f, 0.5f);
    m_program = SimScene::edge_program;
    m_vertex_array = SimScene::vao;
    m_mvp_id = SimScene::edge_mvp_id;
}

void EdgeNode::input() {
}

void EdgeNode::update(UpdateState us) {
}

void EdgeNode::render(RenderState rs) {
}
