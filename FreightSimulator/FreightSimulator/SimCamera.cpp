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

void SimCamera::setup() {
    m_position = glm::vec3(0.0f,0.0f,75.0f);
    m_focal_point = glm::vec3(m_position.x,m_position.y,0.0f);
    m_zoom = 12.0f;
}

void SimCamera::input(InputState is) {

}

void SimCamera::update(UpdateState us) {
    //Lock direction
    m_focal_point.x = m_position.x;
    m_focal_point.y = m_position.y;
}
