//
//  CityNode.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-06.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "CityNode.hpp"
#include "SimScene.hpp"

void CityNode::setup() {
    m_program = SimScene::program;
    m_vertex_array = SimScene::vao;
    m_mvp_id = SimScene::mvp_id;
}

void CityNode::input() {
}

void CityNode::update(double deltaTime) {
}

void CityNode::render(RenderState rs) {
}
