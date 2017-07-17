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

void SimulatorApp::input(InputState is) {
    if (glfwGetKey(is.window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        //Cleanup and exit
        cleanup();
        glfwSetWindowShouldClose(is.window, GLFW_TRUE);
    }
    
    m_scene->_input(is);
}

void SimulatorApp::update(UpdateState us) {
    //Calculate delta time
    m_scene->_update(us);
}

void SimulatorApp::render(RenderState rs) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,0.0);

    m_scene->_render(rs);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}


