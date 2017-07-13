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

    m_root_node = new SceneNode();
    m_root_node->_setup();
    m_root_node->willRender = false;

    setup();
}

void Scene::_input(InputState is) {
    input(is);
    if (m_scene_camera != nullptr) {
        m_scene_camera->_input(is);
    }
    m_root_node->_input(is);
}

void Scene::_update(UpdateState us) {
    //Update camera
    if (m_scene_camera != nullptr) {
        m_scene_camera->_update(us);
    }

    //User update
    update(us);

    //Update root node
    m_root_node->_update(us);
}

void Scene::_render(RenderState rs) {
    //MVP setup
    if (m_scene_camera != nullptr) {
        rs.mvp = rs.mvp * m_scene_camera->vp_matrix;
    }

    //User render
    render(rs);

    //Render root node
    m_root_node->_render(rs);
}

void Scene::attachSceneCamera(Camera* camera) {
    m_scene_camera = camera;
    m_scene_camera->_setup();
}
