//
//  Engine.hpp
//  FreightSimulator
//
//  https://placeofmemory.wordpress.com/2013/11/24/a-very-simple-glfw-project-framework/
//
//  Created by Sean Amadio on 2017-06-08.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef Engine_hpp
#define Engine_hpp

#include <stdio.h>
#include <string>
#include <GLFW/glfw3.h>
#include "App.hpp"

class GLFWEngine
{
public:
    GLFWEngine(unsigned int window_width, unsigned int window_height, unsigned int glMajorVersion,
           unsigned int glMinorVersion, std::string windowTitle);
    ~GLFWEngine();
    void setup(bool fullScreenEnable);
    void attachDemo(App* appInstance);
    void run();
    void cleanup();

private:
    void input();
    void update();
    void render();
    bool m_isFullScreen;
    unsigned int m_windowWidth;
    unsigned int m_windowHeight;
    unsigned int m_glMinorVersion;
    unsigned int m_glMajorVersion;

    std::string m_windowTitle;
    App* m_appInstance;
    GLFWwindow* m_window;
    double lastTime;
};

#endif /* Engine_hpp */
