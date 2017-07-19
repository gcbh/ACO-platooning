//
//  QuadNode.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-18.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "QuadNode.hpp"
#include "glm.hpp"
#include "matrix_transform.hpp"

void QuadNode::render(RenderState rs) {
    glUseProgram(m_program);
    glBindVertexArray(m_vertex_array);
    glUniformMatrix4fv(m_mvp_id, 1, GL_FALSE, &rs.mvp[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
