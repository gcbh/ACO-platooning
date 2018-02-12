//
//  TruckNode.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2018-01-22.
//  Copyright © 2018 FYDP. All rights reserved.
//

#include "TruckNode.hpp"
#include "SimScene.hpp"
#include "glm.hpp"
#include "matrix_transform.hpp"

static const ImVec4 aco_color = ImVec4(1.0f,1.0f,1.0f,1.0f);
static const ImVec4 dijkstra_color = ImVec4(0.5f,0.5f,1.0f,1.0f);

TruckNode::TruckNode(json j) {
    m_id = j.at("vehicle_id").get<int>();
    for (auto& segment : j["segments"]) {
        Segment* s = new Segment(segment);
        m_schedule.push_back(s);
    }
}

void TruckNode::postsetup() {
    m_scale = glm::vec3(0.1f, 0.1f, 0.1f);
    m_program = SimScene::city_program;
    m_vertex_array = SimScene::vao;
    m_mvp_id = SimScene::city_mvp_id;
}

void TruckNode::preinput(InputState* is) {
}

void TruckNode::update(UpdateState* us) {
    
}

void TruckNode::prerender(RenderState* rs) {
}

void TruckNode::render(RenderState* rs) {

    switch (rs->truckMode) {
        case TruckMode::Dijkstra:
            if (m_type == TruckType::Dijkstra) drawTruck(rs);
            break;
        case TruckMode::ACO:
            if (m_type == TruckType::ACO) drawTruck(rs);
            break;
        case TruckMode::DijkstraAndACO:
            drawTruck(rs);
            break;
        default:
            break;
    }
}

void TruckNode::drawTruck(RenderState* rs) {
    ImVec2 point = getScreenSpace(rs);
    switch(m_type) {
        case TruckType::ACO:
            ImGui::GetWindowDrawList()->AddCircleFilled(point, 2.0f, ImColor(aco_color));
            break;
        case TruckType::Dijkstra:
            ImGui::GetWindowDrawList()->AddCircleFilled(point, 2.0f, ImColor(dijkstra_color));
            break;
    }
}


