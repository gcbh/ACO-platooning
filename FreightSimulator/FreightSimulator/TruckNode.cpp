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

static const ImU32 black = ImColor(ImVec4(0.0f,0.0f,0.0f,1.0f));
static const ImU32 white = ImColor(ImVec4(1.0f,1.0f,1.0f,1.0f));
static const ImU32 aco_color = ImColor(ImVec4(0.101f, 0.576f, 1.0f,1.0f));
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
    float timecode = us->sim_time;
    double time = 0.0;

    //Loop through segments and position ourselves
    std::vector<Segment*>::iterator j;
    for (j = m_schedule.begin(); j != m_schedule.end(); j++) {
        Segment* s = *j;

        double segment_end_time = time + s->time;

        //We are on the current segment
        if (timecode <= segment_end_time) {
            double segment_progress = fmax(0.0, timecode - time) / s->time;

            float x = s->start_node->m_position.x + (s->end_node->m_position.x - s->start_node->m_position.x)*segment_progress;
            float y = s->start_node->m_position.y + (s->end_node->m_position.y - s->start_node->m_position.y)*segment_progress;
            m_position = glm::vec3(x,y,0.0f);
            break;
        }

        time = segment_end_time;
    }
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
            ImGui::GetWindowDrawList()->AddCircleFilled(point, 10.0f, white);
            ImGui::GetWindowDrawList()->AddCircleFilled(point, 8.0f, m_highlighted ? aco_color : black);
            break;
        case TruckType::Dijkstra:
            ImGui::GetWindowDrawList()->AddCircleFilled(point, 10.0f, white);
            ImGui::GetWindowDrawList()->AddCircleFilled(point, 8.0f, m_highlighted ? aco_color : black);
            break;
    }
}


