//
//  SimulatorApp.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-06-08.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef SimulatorApp_h
#define SimulatorApp_h

#include "App.hpp"
#include <string>
#include <GLFW/glfw3.h>
#include "SimScene.hpp"

class SimulatorApp : public App
{
public:

    SimulatorApp() : App() {}
    std::string getName();
    void setup();
    void cleanup();
    void input(GLFWwindow *window);
    void update(double deltaTime);
    void render(GLFWwindow *window);

private:
    SimScene* m_scene;
};

#endif /* SimulatorApp_h */
