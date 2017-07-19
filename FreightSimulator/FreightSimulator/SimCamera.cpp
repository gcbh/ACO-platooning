//
//  SimCamera.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-06.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "SimCamera.hpp"

#define CAMERA_SPEED_X 10.0
#define CAMERA_SPEED_Y 10.0
#define CAMERA_SPEED_Z 0.1

void SimCamera::setup() {
    m_position = glm::vec3(0.0f,0.0f,75.0f);
    m_focal_point = glm::vec3(m_position.x,m_position.y,0.0f);
    m_zoom = 0.5f;
}

void SimCamera::input(InputState is) {
    if (glfwGetKey(is.window, GLFW_KEY_UP ) == GLFW_PRESS){
        m_position.y += is.deltaTime * CAMERA_SPEED_Y;
    }
    // Move backward
    if (glfwGetKey(is.window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        m_position.y -= is.deltaTime * CAMERA_SPEED_Y;
    }
    // Strafe right
    if (glfwGetKey(is.window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        m_position.x += is.deltaTime * CAMERA_SPEED_X;
    }
    // Strafe left
    if (glfwGetKey(is.window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        m_position.x -= is.deltaTime * CAMERA_SPEED_X;
    }
    // Zoom In
    if (glfwGetKey(is.window, GLFW_KEY_EQUAL ) == GLFW_PRESS){
        m_zoom -= is.deltaTime * CAMERA_SPEED_Z;
    }
    // Zoom Out
    if (glfwGetKey(is.window, GLFW_KEY_MINUS ) == GLFW_PRESS){
        m_zoom += is.deltaTime * CAMERA_SPEED_Z;
    }
}

void SimCamera::update(UpdateState us) {
    //Lock direction
    m_focal_point.x = m_position.x;
    m_focal_point.y = m_position.y;
}
