//
//  App.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-06-08.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "SimulatorApp.hpp"
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include "glm.hpp"

std::string SimulatorApp::getName(){
    return "Automated Freight Transport Simulator";
}

void SimulatorApp::setup() {
    m_scene = new SimScene();
    m_scene->_setup();
}

void SimulatorApp::cleanup() {
    fprintf(stdout, "App Cleanup.\n");
}

void SimulatorApp::input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        //Cleanup and exit
        cleanup();
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    m_scene->_input();
}

void SimulatorApp::update(double deltaTime) {
    //Calculate delta time
    m_scene->_update(deltaTime);
}

void SimulatorApp::render(GLFWwindow *window) {

    RenderState rs = {window, glm::mat4(1.0f)};
    m_scene->_render(rs);
}


