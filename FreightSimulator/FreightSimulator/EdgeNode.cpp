//
//  EdgeNode.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-17.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "EdgeNode.hpp"
#include "SimScene.hpp"
#include "glm.hpp"
#include "matrix_transform.hpp"
#include "Heatmap.hpp"

static const ImU32 black = ImColor(ImVec4(0.0f,0.0f,0.0f,1.0f));

void EdgeNode::postsetup() {
}

void EdgeNode::preinput(InputState* is) {
}

void EdgeNode::preupdate(UpdateState* us) {
}

void EdgeNode::prerender(RenderState* rs) {
}

void EdgeNode::render(RenderState* rs) {
    ImVec4 staticCol = generate_static_color(rs);
    ImVec4 dynamicCol = generate_dynamic_color(rs);

    // Move out to world space
    rs->mvp = rs->mvp * glm::inverse(m_model_matrix);
    ImVec2 ss1 = getScreenSpace(rs, m_position_pair.first);
    ImVec2 ss2 = getScreenSpace(rs, m_position_pair.second);
    ImVec2 ssm = getScreenSpace(rs, glm::vec4(m_position, 1.0f));

    switch (rs->roadMode) {
        case RoadMode::Default:
            ImGui::GetWindowDrawList()->AddLine(ss1, ss2, black, 3.0);
            break;
        case RoadMode::StaticHeat:
            ImGui::GetWindowDrawList()->AddLine(ss1, ss2, ImColor(staticCol), 3.0);
            break;
        case RoadMode::DynamicHeat:
            ImGui::GetWindowDrawList()->AddLine(ss1, ss2, ImColor(dynamicCol), 3.0);
            break;
        default:
            break;
    }

    ImVec2 textPoint = ImVec2(ssm.x+2, ssm.y-6);
    std::ostringstream ss;
    switch (rs->roadLabelMode) {
        case RoadLabelMode::Distance:
            ss << m_weight;
            ImGui::GetWindowDrawList()->AddText(textPoint, black, ss.str().c_str());
            break;
        case RoadLabelMode::StaticHeat:
            ss << m_static_heat;
            ImGui::GetWindowDrawList()->AddText(textPoint, ImColor(staticCol), ss.str().c_str());
            break;
        case RoadLabelMode::DynamicHeat:
            ss << m_dynamic_heat;
            ImGui::GetWindowDrawList()->AddText(textPoint, ImColor(dynamicCol), ss.str().c_str());
            break;
        default:
            break;
    }
    rs->mvp = rs->mvp * m_model_matrix;
}

ImVec4 EdgeNode::generate_static_color(RenderState* rs) {
    //Grab the heat values for this edge
    float aco_heat = (*rs->aco_static_heatmap)[m_id];
    float dijkstra_heat = (*rs->dijkstra_static_heatmap)[m_id];

    //Init colour and gradient values
    float r = 1.0;
    float g = 1.0;
    float b = 1.0;

    // Generate the static heat value depending on the mode
    switch(rs->truckMode) {
        case TruckMode::Dijkstra:
            Heatmap::getColorAtValue(dijkstra_heat,r,g,b);
            break;
        case TruckMode::ACO:
            Heatmap::getColorAtValue(aco_heat,r,g,b);
            break;
        case TruckMode::DijkstraAndACO:
            Heatmap::getColorAtValue((aco_heat+dijkstra_heat)/2.0,r,g,b);
            break;
        default:
            break;
    }

    return ImVec4(r,g,b,1.0f);
}

ImVec4 EdgeNode::generate_dynamic_color(RenderState* rs) {
    //Grab the heat values for this edge
    float aco_heat = (*rs->aco_dynamic_heatmap)[m_id];
    float dijkstra_heat = (*rs->dijkstra_dynamic_heatmap)[m_id];

    //Init colour and gradient values
    float r = 1.0;
    float g = 1.0;
    float b = 1.0;

    // Generate the static heat value depending on the mode
    switch(rs->truckMode) {
        case TruckMode::Dijkstra:
            Heatmap::getColorAtValue(dijkstra_heat,r,g,b);
            break;
        case TruckMode::ACO:
            Heatmap::getColorAtValue(aco_heat,r,g,b);
            break;
        case TruckMode::DijkstraAndACO:
            Heatmap::getColorAtValue((aco_heat+dijkstra_heat)/2.0,r,g,b);
            break;
        default:
            break;
    }

    return ImVec4(r,g,b,1.0f);
}
