//
//  Scene.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-06-12.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "Scene.hpp"

Scene::Scene() {
}

Scene::~Scene() { }

void Scene::_setup() {
    //Init with identity matrix by default
    m_projection_matrix = glm::mat4(1.0f);
    m_view_matrix = glm::mat4(1.0f);

    m_root_node = new SceneNode();
    m_root_node->_setup();
    m_root_node->willRender = false;

    setup();
}

void Scene::_input() {
    input();
    m_root_node->_input();
}

void Scene::_update(double deltaTime) {
    //Update camera


    //User update
    update(deltaTime);

    UpdateState us = {deltaTime};
    //Update root node
    m_root_node->_update(us);
}

void Scene::_render(RenderState rs) {
    //MVP setup
    glm::mat4 vp_matrix = m_projection_matrix * m_view_matrix;
    rs.mvp = vp_matrix;

    //User render
    render(rs);

    //Render root node
    m_root_node->_render(rs);
}
