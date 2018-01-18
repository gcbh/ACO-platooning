//
//  Engine.cpp
//  FreightSimulator
//
//  https://placeofmemory.wordpress.com/2013/11/24/a-very-simple-glfw-project-framework/
//
//  Created by Sean Amadio on 2017-06-08.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "GLFWEngine.hpp"
#include "glm.hpp"
#include <stdlib.h>

GLFWEngine::GLFWEngine(unsigned int window_width, unsigned int window_height, unsigned int glMajorVersion, unsigned int glMinorVersion, std::string windowTitle) {
    m_windowWidth = window_width;
    m_windowHeight = window_height;
    m_glMajorVersion = glMajorVersion;
    m_glMinorVersion = glMinorVersion;
    m_windowTitle = windowTitle;
    lastTime = glfwGetTime();
    deltaTime = 0.0;
}

GLFWEngine::~GLFWEngine() { }

void GLFWEngine::attachDemo(App* appInstance) {
    m_appInstance = appInstance;
}

void GLFWEngine::setup(bool fullScreenEnable)
{
    //Attempt to init GLFW
    if(!glfwInit()) {
        fprintf(stderr,"Error: Failed to initialize glfw.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    //Set GLFW window parameters
    glfwWindowHint(GLFW_SAMPLES, 1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_glMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_glMinorVersion);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Attempt to create GLFW window
    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle.c_str(), fullScreenEnable ? glfwGetPrimaryMonitor() : NULL, NULL);
    if(!m_window) {
        glfwTerminate();
        fprintf(stderr,"Error: Could not create window context.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    //glfwSetWindowSizeCallback(m_window, window_size_callback);
    glfwMakeContextCurrent(m_window);

    fprintf(stdout, "Engine Setup Complete.\n");

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(m_window, true);

    // Setup style
    ImGui::StyleColorsClassic();

    fprintf(stdout, "UI Setup Complete.\n");

    // Setup our attached demo if it exists
    if(m_appInstance != NULL) {
        m_appInstance->setup();
    }
}

void GLFWEngine::run() {

    fprintf(stdout, "Engine Starting.\n");
    while (!glfwWindowShouldClose(m_window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        input();
        update();
        render();

        ImGui::Render();
        glfwSwapBuffers(m_window);

    }
    cleanup();
}

void GLFWEngine::input() {
    //Calculate delta time
    double currentTime = glfwGetTime();
    deltaTime = double(currentTime - lastTime);
    lastTime = currentTime;

    //Handle app input
    InputState is = {m_window, deltaTime};
    m_appInstance->input(is);
}

void GLFWEngine::update() {


    //Update app
    UpdateState us = {deltaTime};
    m_appInstance->update(us);
}

void GLFWEngine::render() {
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    glViewport(0, 0, width, height);

    //Render app
    RenderState rs = {m_window, RenderMode::Normal, glm::mat4(1.0f)};
    m_appInstance->render(rs);
}

void GLFWEngine::cleanup() {
    fprintf(stdout, "Engine Cleanup.\n");
    
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
