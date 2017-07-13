//
//  SimScene.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-05.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "SimScene.hpp"
#include "ShaderUtils.hpp"
#include "TextureUtils.hpp"
#include "glm.hpp"
#include "CityNode.hpp"
#include "SimCamera.hpp"
#include "lodepng.h"

//Basic quad
static const GLfloat points[] = {
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.0f, 0.0f, 1.0f
};

GLuint SimScene::vbo = 0;
GLuint SimScene::vao = 0;
GLuint SimScene::program = 0;
GLuint SimScene::tex = 0;
GLuint SimScene::mvp_id = 0;

void SimScene::setup() {
    SimCamera* camera = new SimCamera();
    attachSceneCamera(camera);

    //Quad VBO
    glGenBuffers(1, &SimScene::vbo);
    glBindBuffer(GL_ARRAY_BUFFER, SimScene::vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    //Quad VAO
    glGenVertexArrays(1, &SimScene::vao);
    glBindVertexArray(SimScene::vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, SimScene::vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 5*sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 5*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, false, 6*sizeof(GL_FLOAT), 3*sizeof(GL_FLOAT));

    //Basic Shaders
    SimScene::program = LoadShaders("basic_vs.glsl", "basic_fs.glsl");

    //Textures
    SimScene::tex = LoadTexture("circle.png");

    //Get MVP ID
    SimScene::mvp_id = glGetUniformLocation(SimScene::program, "MVP");

    CityNode* city = new CityNode();
    m_root_node->addChildNode(city);
    //city->m_position.x = 10.0;

    CityNode* city2 = new CityNode();
    m_root_node->addChildNode(city2);
    city2->m_position.y = 10.0;

    CityNode* city3 = new CityNode();
    m_root_node->addChildNode(city3);
    city3->m_position.x = 10.0;
    city3->m_position.y = 10.0;
}

void SimScene::input(InputState is) {
    
}

void SimScene::update(UpdateState us) {

}

void SimScene::render(RenderState rs) {
}
