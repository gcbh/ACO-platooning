//
//  App.hpp
//  FreightSimulator
//
//  https://placeofmemory.wordpress.com/2013/11/24/a-very-simple-glfw-project-framework/
//
//  Created by Sean Amadio on 2017-06-08.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef App_hpp
#define App_hpp

#include <stdio.h>
#include <string>
#include <GLFW/glfw3.h>

class App
{
public:
    App() {}
    virtual std::string getName() = 0;
    virtual void setup() = 0;
    virtual void cleanup() = 0;
    virtual void input(GLFWwindow *window) = 0;
    virtual void update(double deltaTime) = 0;
    virtual void render(GLFWwindow *window) = 0;
};

#endif /* App_hpp */
