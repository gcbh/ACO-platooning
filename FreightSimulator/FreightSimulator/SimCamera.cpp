//
//  SimCamera.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-06.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "SimCamera.hpp"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#define CAMERA_SPEED_X 200.0
#define CAMERA_SPEED_Y 200.0
#define CAMERA_SPEED_Z 20.0

ImVec2 prevMouseDrag = ImVec2();

void SimCamera::setup() {
    m_position = glm::vec3(0.0f,0.0f,75.0f);
    m_focal_point = glm::vec3(m_position.x,m_position.y,0.0f);
    m_zoom = 12.0f;
}

void SimCamera::input(InputState is) {

    ImGuiIO& io = ImGui::GetIO();
    float mouseWheel = io.MouseWheel;
    m_zoom -= mouseWheel;

    ImVec2 mouseDrag = ImGui::GetMouseDragDelta(0, 0.0f);
    ImVec2 mouseDragDelta = ImVec2(mouseDrag.x - prevMouseDrag.x, mouseDrag.y - prevMouseDrag.y);

    if (!ImGui::IsMouseReleased(0)) {
        m_position.x -= mouseDragDelta.x/m_zoom;
        m_position.y += mouseDragDelta.y/m_zoom;
    }

    prevMouseDrag = mouseDrag;
}

void SimCamera::update(UpdateState us) {
    //Lock direction
    m_focal_point.x = m_position.x;
    m_focal_point.y = m_position.y;
}
