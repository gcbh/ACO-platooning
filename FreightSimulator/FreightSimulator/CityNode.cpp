//
//  CityNode.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-06.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "CityNode.hpp"
#include "SimScene.hpp"
#include "glm.hpp"
#include "matrix_transform.hpp"
#include <iostream>
#include <sstream>
#include <string>

void CityNode::postsetup() {
    m_scale = glm::vec3(0.1f, 0.1f, 0.1f);
    m_program = SimScene::city_program;
    m_vertex_array = SimScene::vao;
    m_mvp_id = SimScene::city_mvp_id;
}

void CityNode::preinput(InputState is) {

}

void CityNode::preupdate(UpdateState us) {

}

void CityNode::postupdate(UpdateState us) {

}
