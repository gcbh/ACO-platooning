//
//  SimScene.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-05.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "SimScene.hpp"
#include "ShaderUtils.hpp"
#include "glm.hpp"
#include "CityNode.hpp"

//Basic quad
static const GLfloat points[] = {
    0.5f,  0.5f,  0.0f,
    0.5f, -0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f,
    0.5f,  0.5f,  0.0f,
    -0.5f, 0.5f,  0.0f,
    -0.5f, -0.5f,  0.0f
};

GLuint SimScene::vbo = 0;
GLuint SimScene::vao = 0;
GLuint SimScene::program = 0;
GLuint SimScene::mvp_id = 0;

void SimScene::setup() {
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    //m_projection_matrix = glm::perspective(glm::radians(45.0f), (float) (800.0 / 600.0), 0.1f, 100.0f);

    // Or, for an ortho camera :
    //m_projection_matrix = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    /*m_view_matrix = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );*/

    //Quad VBO
    glGenBuffers(1, &SimScene::vbo);
    glBindBuffer(GL_ARRAY_BUFFER, SimScene::vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    //Quad VAO
    glGenVertexArrays(1, &SimScene::vao);
    glBindVertexArray(SimScene::vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, SimScene::vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //Basic Shaders
    SimScene::program = LoadShaders("basic_vs.glsl", "basic_fs.glsl");

    //Get MVP ID
    SimScene::mvp_id = glGetUniformLocation(SimScene::program, "MVP");

    CityNode* city = new CityNode();
    m_root_node->addChildNode(city);

    fprintf(stdout, "SimScene User Setup.\n");
}

void SimScene::input() {
}

void SimScene::update(double deltaTime) {
}

void SimScene::render(RenderState rs) {
}
