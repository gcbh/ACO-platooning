//
//  QuadNode.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-18.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "QuadNode.hpp"
#include "ShaderUtils.hpp"
#include "glm.hpp"
#include "matrix_transform.hpp"

//Basic quad
static const GLfloat quad[] = {
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.0f, 0.0f, 1.0f
};

GLuint QuadNode::vbo = -1;
GLuint QuadNode::vao = -1;
GLuint QuadNode::program = -1;

void QuadNode::setup() {

    //One time setup the quad vertices
    if (QuadNode::vao == -1) {
        //Quad VBO
        glGenBuffers(1, &QuadNode::vbo);
        glBindBuffer(GL_ARRAY_BUFFER, QuadNode::vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

        //Quad VAO
        glGenVertexArrays(1, &QuadNode::vao);
        glBindVertexArray(QuadNode::vao);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, QuadNode::vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 5*sizeof(GL_FLOAT), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 5*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(1);
    }

    //One time setup the basic texturing program
    if (QuadNode::program == -1) {
        QuadNode::program = LoadShaders("basic_vs.glsl", "basic_fs.glsl");
    }
}

void QuadNode::render(RenderState rs) {
    glUseProgram(QuadNode::program);
    glBindVertexArray(QuadNode::vao);
    glUniformMatrix4fv(m_mvp_id, 1, GL_FALSE, &rs.mvp[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
