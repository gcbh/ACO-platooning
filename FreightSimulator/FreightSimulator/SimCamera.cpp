//
//  SimCamera.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-06.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "SimCamera.hpp"
#define CAMERA_SPEED 50.0

void SimCamera::setup() {
    m_position = glm::vec3(-98.5795,39.8283,75);
    m_focal_point = glm::vec3(m_position.x,m_position.y,0.0f);
    m_fov = 45.0;
}

void SimCamera::input(InputState is) {
    if (glfwGetKey(is.window, GLFW_KEY_UP ) == GLFW_PRESS){
        m_position.y += is.deltaTime * CAMERA_SPEED;
    }
    // Move backward
    if (glfwGetKey(is.window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        m_position.y -= is.deltaTime * CAMERA_SPEED;
    }
    // Strafe right
    if (glfwGetKey(is.window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        m_position.x += is.deltaTime * CAMERA_SPEED;
    }
    // Strafe left
    if (glfwGetKey(is.window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        m_position.x -= is.deltaTime * CAMERA_SPEED;
    }
    // Zoom In
    if (glfwGetKey(is.window, GLFW_KEY_EQUAL ) == GLFW_PRESS){
        m_position.z -= is.deltaTime * CAMERA_SPEED;
    }
    // Zoom Out
    if (glfwGetKey(is.window, GLFW_KEY_MINUS ) == GLFW_PRESS){
        m_position.z += is.deltaTime * CAMERA_SPEED;
    }
}

void SimCamera::update(UpdateState us) {
    //Lock direction
    m_focal_point.x = m_position.x;
    m_focal_point.y = m_position.y;
}
