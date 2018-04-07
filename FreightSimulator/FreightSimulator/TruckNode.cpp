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
#include <sstream>
#include <iostream>

static const ImU32 black = ImColor(ImVec4(0.0f,0.0f,0.0f,1.0f));
static const ImU32 white = ImColor(ImVec4(1.0f,1.0f,1.0f,1.0f));
static const ImU32 green = ImColor(ImVec4(0.556f, 1.0f, 0.443f,1.0f));
static const ImU32 aco_color = ImColor(ImVec4(0.101f, 0.576f, 1.0f,1.0f));
static const ImVec4 dijkstra_color = ImVec4(0.5f,0.5f,1.0f,1.0f);

static const float circle_base_size = 10.0f;
static const float circle_complete_size = 5.0f;
static const float circle_stroke_width = 2.0f;


TruckNode::TruckNode(json j) {
    m_id = j.at("vehicle_id").get<int>();
    for (auto& segment : j["segments"]) {
        Segment* s = new Segment(segment);
        m_schedule.push_back(s);
    }
    current_segment = nullptr;
    m_state = TruckState::Waiting;
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

        double weighting_end_time = time + s->max_wait;
        double segment_end_time = time + s->max_wait + s->time;

        //If we are in the current segment waiting period
        if (timecode <= weighting_end_time) {
            float x = s->start_node->m_position.x;
            float y = s->start_node->m_position.y;
            m_position = glm::vec3(x,y,0.0f);
            current_segment = s;
            m_state = TruckState::Waiting;
            return;
        }

        //We are on the current segment
        if (timecode <= segment_end_time) {
            double segment_progress = fmax(0.0, timecode - weighting_end_time) / s->time;

            float x = s->start_node->m_position.x + (s->end_node->m_position.x - s->start_node->m_position.x)*segment_progress;
            float y = s->start_node->m_position.y + (s->end_node->m_position.y - s->start_node->m_position.y)*segment_progress;
            m_position = glm::vec3(x,y,0.0f);
            current_segment = s;
            m_state = TruckState::Driving;
            return;
        }

        time = segment_end_time;
    }

    //We are at the end of the schedule
    float x = m_schedule.back()->end_node->m_position.x;
    float y = m_schedule.back()->end_node->m_position.y;
    m_position = glm::vec3(x,y,0.0f);
    m_state = TruckState::Complete;
    current_segment = nullptr;
}

void TruckNode::prerender(RenderState* rs) {
}

void TruckNode::render(RenderState* rs) {

    switch (rs->truckMode) {
        case TruckType::Dijkstra:
            if (m_type == TruckType::Dijkstra) drawTruck(rs);
            break;
        case TruckType::ACO:
            if (m_type == TruckType::ACO) drawTruck(rs);
            break;
    }
}

void TruckNode::drawTruck(RenderState* rs) {

    int labelSize = 1;
    if (current_segment != nullptr) {

        int platoonSize = (int)current_segment->platoon_members.size()+1;

        if (m_state == TruckState::Waiting) {
            // We are waiting, see how many others are waiting
            for (int i = 0; i < platoonSize-1; i++) {
                TruckNode* truck = SimScene::aco_truck_map[current_segment->platoon_members[i]];

                if (current_segment->isEqual(truck->current_segment) &&
                    truck->m_state == TruckState::Waiting) {
                    labelSize++;
                }
            }
        } else {
            // We are in motion and if we are not the leader don't render
            labelSize = platoonSize;
            if (platoonSize > 1) {
                // If we are in a platoon check to see if we are the leader (lowest ID)
                int potentialLeaderID = *min_element(std::begin(current_segment->platoon_members), std::end(current_segment->platoon_members));
                if (potentialLeaderID < m_id) {
                    return;
                }
            }
        }
    }

    ImU32 color = (SimScene::selected == this || SimScene::highlighted == this) ? aco_color : ((m_state == TruckState::Complete) ? green : black);
    float size = (m_state == TruckState::Complete) ? circle_complete_size : circle_base_size;
    std::ostringstream ss;
    ss << labelSize;

    ImVec2 point = getScreenSpace(rs);
    ImVec2 stringSize = ImGui::CalcTextSize(ss.str().c_str());
    ImVec2 textPoint = ImVec2(point.x-stringSize.x/2.0,point.y-stringSize.y/2.0);

    switch(m_type) {
        case TruckType::ACO:
            ImGui::GetWindowDrawList()->AddCircleFilled(point, size, white);
            ImGui::GetWindowDrawList()->AddCircleFilled(point, size-circle_stroke_width, color);
            if (m_state != TruckState::Complete) {
                ImGui::GetWindowDrawList()->AddText(textPoint, white, ss.str().c_str());
            }
            break;
        case TruckType::Dijkstra:
            ImGui::GetWindowDrawList()->AddCircleFilled(point, size, white);
            ImGui::GetWindowDrawList()->AddCircleFilled(point, size-circle_stroke_width, color);
            if (m_state != TruckState::Complete) {
                ImGui::GetWindowDrawList()->AddText(textPoint, white, ss.str().c_str());
            }
            break;
    }
}


