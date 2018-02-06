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

static const ImVec4 col = ImVec4(1.0f,1.0f,1.0f,1.0f);
static const ImU32 col32 = ImColor(col);

void CityNode::postsetup() {
    m_scale = glm::vec3(0.1f, 0.1f, 0.1f);
    m_program = SimScene::city_program;
    m_vertex_array = SimScene::vao;
    m_mvp_id = SimScene::city_mvp_id;
}

void CityNode::preinput(InputState* is) {

}

void CityNode::preupdate(UpdateState* us) {

}

void CityNode::postupdate(UpdateState* us) {

}

void CityNode::render(RenderState* rs) {
    ImVec2 point = getScreenSpace(rs);
    switch (rs->cityMode) {
        case CityMode::Default:
            ImGui::GetWindowDrawList()->AddCircleFilled(point, 3.0f, col32);
            break;
        default:
            break;
    }


    ImVec2 textPoint = ImVec2(point.x+2, point.y-6);
    std::ostringstream ss;
    switch (rs->cityLabelMode) {
        case CityLabelMode::ID:
            ss << m_id;
            ImGui::GetWindowDrawList()->AddText(textPoint, col32, ss.str().c_str());
            break;
        case CityLabelMode::Name:
            ss << m_name;
            ImGui::GetWindowDrawList()->AddText(textPoint, col32, ss.str().c_str());
            break;
        case CityLabelMode::NameAndID:
            ss << m_id << ": " << m_name;
            ImGui::GetWindowDrawList()->AddText(textPoint, col32, ss.str().c_str());
            break;
        default:
            break;
    }
}

