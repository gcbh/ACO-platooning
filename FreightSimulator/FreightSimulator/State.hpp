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
#include <map>
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
    Distance,
    StaticHeat,
    DynamicHeat
};

enum class TruckMode : int {
    None,
    Dijkstra,
    ACO,
    DijkstraAndACO
};

enum class TruckType : int {
    Dijkstra,
    ACO
};

struct InputState {
    GLFWwindow *window;
    double deltaTime;
};

struct UpdateState {
    double deltaTime;
    float sim_time;
    float sim_time_scale;
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

    std::map<int, float> *aco_static_heatmap;
    std::map<int, float> *dijkstra_static_heatmap;
    std::map<int, float> *aco_dynamic_heatmap;
    std::map<int, float> *dijkstra_dynamic_heatmap;

    // Sim Info
    float sim_time;
    float sim_time_scale;
};

#endif /* RenderState_hpp */
