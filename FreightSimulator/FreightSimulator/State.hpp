//
//  RenderState.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-05.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef RenderState_hpp
#define RenderState_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>
#include "glm.hpp"

enum class RenderMode {
    Normal,
    StaticHeat,
    DynamicHeat
};

struct InputState {
    GLFWwindow *window;
    double deltaTime;
};

struct UpdateState {
    double deltaTime;
};

struct RenderState {
    GLFWwindow *window;
    RenderMode mode;
    glm::mat4 mvp;
};

#endif /* RenderState_hpp */
