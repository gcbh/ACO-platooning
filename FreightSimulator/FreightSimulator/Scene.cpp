//
//  Scene.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-06-12.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "Scene.hpp"
#include "TextureUtils.hpp"

Scene::Scene() {
}

Scene::~Scene() { }

void Scene::_setup() {

    m_root_node = new SceneNode();
    m_root_node->_setup();
    m_root_node->parentScene = this;
    m_root_node->willRender = false;

    presetup();
    setup();
    postsetup();
}

void Scene::_input(InputState is) {

    preinput(is);
    input(is);
    postinput(is);

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
    preupdate(us);
    update(us);
    postupdate(us);

    //Update root node
    m_root_node->_update(us);
}

void Scene::_render(RenderState* rs) {
    //MVP setup
    if (m_scene_camera != nullptr) {
        rs->mvp = rs->mvp * m_scene_camera->vp_matrix;
    }

    //User render
    prerender(rs);
    render(rs);
    m_root_node->_render(rs);
    postrender(rs);

    //Render root node

}

void Scene::attachSceneCamera(Camera* camera) {
    m_scene_camera = camera;
    m_scene_camera->_setup();
}

void Scene::addTexture(std::string file_name) {
    texture_map[file_name] = LoadTexture(file_name);
}

GLuint Scene::getTexture(std::string file_name) {
    return texture_map[file_name];
}
