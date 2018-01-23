//
//  RenderState.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-05.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef RenderState_hpp
#define RenderState_hpp

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "glm.hpp"

enum class CityMode : int {
    None,
    Default
};

enum class CityLabelMode : int {
    None,
    Name,
    ID,
    NameAndID
};

enum class RoadMode : int {
    None,
    Default,
    StaticHeat,
    DynamicHeat
};

enum class RoadLabelMode : int {
    None,
    Distance
};

enum class TruckMode : int {
    None,
    Dijkstra,
    ACO,
    DijkstraAndACO
};

struct InputState {
    GLFWwindow *window;
    double deltaTime;
};

struct UpdateState {
    double deltaTime;
};

struct RenderState {
    // Rendering
    GLFWwindow *window;
    glm::mat4 mvp;
    ImVec2 screen_size;

    // Rendering Modes
    CityMode cityMode;
    CityLabelMode cityLabelMode;
    RoadMode roadMode;
    RoadLabelMode roadLabelMode;
    TruckMode truckMode;
};

#endif /* RenderState_hpp */
