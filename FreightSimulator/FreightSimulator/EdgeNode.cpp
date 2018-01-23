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

static const ImVec4 col = ImVec4(1.0f,1.0f,1.0f,1.0f);
static const ImU32 col32 = ImColor(col);

void EdgeNode::postsetup() {
}

void EdgeNode::preinput(InputState is) {
}

void EdgeNode::preupdate(UpdateState us) {
}

void EdgeNode::prerender(RenderState* rs) {
}

void EdgeNode::render(RenderState* rs) {

    // Move out to world space
    rs->mvp = rs->mvp * glm::inverse(m_model_matrix);
    ImVec2 ss1 = getScreenSpace(rs, m_position_pair.first);
    ImVec2 ss2 = getScreenSpace(rs, m_position_pair.second);
    ImVec2 ssm = getScreenSpace(rs, glm::vec4(m_position, 1.0f));

    switch (rs->roadMode) {
        case RoadMode::Default:
            ImGui::GetWindowDrawList()->AddLine(ss1, ss2, col32);
            break;
        default:
            break;
    }


    ImVec2 textPoint = ImVec2(ssm.x+2, ssm.y-6);
    std::ostringstream ss;
    switch (rs->roadLabelMode) {
        case RoadLabelMode::Distance:
            ss << m_weight;
            ImGui::GetWindowDrawList()->AddText(textPoint, col32, ss.str().c_str());
            break;
        default:
            break;
    }
    rs->mvp = rs->mvp * m_model_matrix;
}
