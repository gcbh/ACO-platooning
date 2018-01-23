//
//  Camera.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-06.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "Camera.hpp"

Camera::Camera() {}
Camera::~Camera() {}

void Camera::_setup() {
    //Init with identity matrix by default
    vp_matrix = glm::mat4(1.0f);

    m_position = glm::vec3(0.0f);
    m_focal_point = glm::vec3(0.0f);
    m_width = 1600.0;
    m_height = 900.0;
    m_zoom = 1.0f;

    setup();
}

void Camera::_input(InputState is) {
    input(is);
}

void Camera::_update(UpdateState us) {
    //Update camera
    m_position = m_position + (m_velocity * (float)us.deltaTime);

    //glm::mat4 projection_matrix = glm::perspective(glm::radians(m_fov), (float) (800.0 / 600.0), 0.1f, 1000.0f);
    glm::mat4 projection_matrix = glm::ortho( (-m_width/2.0f)/m_zoom, (m_width/2.0f)/m_zoom, (-m_height/2.0f)/m_zoom, (m_height/2.0f)/m_zoom, 0.0f, 100.0f);

    glm::mat4 view_matrix = glm::lookAt(
        m_position, // Camera is at (4,3,3), in World Space
        m_focal_point, // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    update(us);

    vp_matrix = projection_matrix * view_matrix;
}
