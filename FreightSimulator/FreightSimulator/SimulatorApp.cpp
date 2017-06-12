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
#include "ShaderUtils.hpp"
#include "linmath.h"

// An array of 3 vectors which represents 3 vertices
static const GLfloat points[] = {
    0.5f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f,
    0.5f,  0.5f,  0.0f,
    -0.5f, 0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
};

std::string SimulatorApp::getName(){
    return "Automated Freight Transport Simulator";
}

void SimulatorApp::setup() {
    m_vbo = 0;
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW);

    m_vao = 0;
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    m_program = LoadShaders("basic_vs.glsl", "basic_fs.glsl");

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
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
}

void SimulatorApp::update(double deltaTime) {

}

void SimulatorApp::render(GLFWwindow *window) {
    glUseProgram(m_program);
    glBindVertexArray(m_vao);
    // draw points 0-3 from the currently bound VAO with current in-use shader
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


