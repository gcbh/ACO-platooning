//
//  SimScene.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-05.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "SimScene.hpp"
#include "ShaderUtils.hpp"
#include "TextureUtils.hpp"
#include "glm.hpp"
#include "SimCamera.hpp"
#include "lodepng.h"
#include <math.h>

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
GLuint SimScene::city_program = 0;
GLuint SimScene::edge_program = 0;
GLuint SimScene::tex = 0;
GLuint SimScene::city_mvp_id = 0;
GLuint SimScene::edge_mvp_id = 0;

void SimScene::postsetup() {
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

    //Basic Shaders
    SimScene::city_program = LoadShaders("basic_vs.glsl", "basic_fs.glsl");
    SimScene::edge_program = LoadShaders("basic_vs.glsl", "edge_fs.glsl");

    //Textures
    addTexture("heat.png");
    addTexture("circle.png");

    //Get MVP IDs
    SimScene::city_mvp_id = glGetUniformLocation(SimScene::city_program, "MVP");
    SimScene::edge_mvp_id = glGetUniformLocation(SimScene::edge_program, "MVP");
}

void SimScene::preinput(InputState is) {
    if (glfwGetKey(is.window, GLFW_KEY_SPACE ) == GLFW_PRESS) {
        std::string closestCity = "";
        float closestDistance = 200.0;
        std::map<int, CityNode*>::iterator it;
        for (it = city_map.begin(); it != city_map.end(); it++) {
            float deltaX = it->second->m_position.x - m_scene_camera->m_position.x;
            float deltaY = it->second->m_position.y - m_scene_camera->m_position.y;
            float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

            if (distance < closestDistance) {
                closestCity = it->second->m_name;
                closestDistance = distance;
            }
        }
        std::cout << "City: " << closestCity << std::endl;
    }
}

void SimScene::preupdate(UpdateState us) {
    ImGui::Begin("Freight Simulator");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    if (ImGui::Button("Load Graph")) {
        loadGraph();
    }

    if (ImGui::Button("Load Manifest")) {
        loadManifest();
    }
    ImGui::End();
}

void SimScene::postupdate(UpdateState us) {
    closestCity();
}

void SimScene::prerender(RenderState rs) {
}

void SimScene::closestCity() {

    if (city_map.size() <= 0) {
        return;
    }
    // Find closest city
    CityNode* closestCity = 0;
    float closestDistance = 1000.0;
    std::map<int, CityNode*>::iterator it;
    for (it = city_map.begin(); it != city_map.end(); it++) {
        float deltaX = it->second->m_position.x - m_scene_camera->m_position.x;
        float deltaY = it->second->m_position.y - m_scene_camera->m_position.y;
        float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

        if (distance < closestDistance) {
            closestCity = it->second;
            closestDistance = distance;
        }
    }

    bool show = false;
    if (closestCity) {
        show = true;
    }

    // Render UI for closest city
    const float DISTANCE = 10.0f;
    static int corner = 3;
    ImVec2 window_pos = ImVec2((corner & 1) ? ImGui::GetIO().DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? ImGui::GetIO().DisplaySize.y - DISTANCE : DISTANCE);
    ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.3f)); // Transparent background
    if (ImGui::Begin("Example: Fixed Overlay", &show, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("%d: %s", closestCity->m_id, closestCity->m_name.c_str());
        ImGui::Separator();
        ImGui::Text("Lat/Long: (%.1f,%.1f)", closestCity->m_position.x, closestCity->m_position.y);
        ImGui::End();
    }
    ImGui::PopStyleColor();
}

void SimScene::loadGraph() {
    //Load cities, also get a middle point to put the camera at to start
    std::ifstream cities_file("cities_ids_coords.txt");
    std::string   line;
    float         avgX = 0.0f;
    float         avgY = 0.0f;

    while(getline(cities_file, line)) {
        std::stringstream   linestream(line);
        int            city_id;
        std::string    city_name;
        float          latitude;
        float          longitude;


        linestream >> city_id >> city_name >> latitude >> longitude;

        CityNode* city = new CityNode();
        m_root_node->addChildNode(city);
        city->m_id = city_id;
        city->m_name = city_name;
        city->m_position.x = longitude;
        city->m_position.y = latitude;

        //Add to our city map
        city_map[city->m_id] = city;
        avgX += longitude;
        avgY += latitude;
    }

    avgX /= city_map.size();
    avgY /= city_map.size();
    m_scene_camera->m_position.x = avgX;
    m_scene_camera->m_position.y = avgY;
    m_scene_camera->m_focal_point.x = avgX;
    m_scene_camera->m_focal_point.y = avgY;

    //Load edges
    std::ifstream edges_file("cities_p.txt");
    int id = 0;

    while(getline(edges_file, line)) {
        std::stringstream   linestream(line);
        int            city_id1;
        std::string    city_name1;
        int            city_id2;
        std::string    city_name2;
        float          weight;

        bool hasBoth = true;

        linestream >> city_id1 >> city_name1 >> city_id2 >> city_name2 >> weight;

        //Check the city list for both cities before creating the edge, log missing ones
        if(city_map.find(city_id1) == city_map.end()) {
            std::cout << "City: " << city_id1 << " " << city_name1 << " is in the edge map but not the city map" << std::endl;
            hasBoth = false;
        }

        if(city_map.find(city_id2) == city_map.end()) {
            std::cout << "City: " << city_id2 << " " << city_name2 << " is in the edge map but not the city map" << std::endl;
            hasBoth = false;
        }

        if (hasBoth) {
            EdgeNode* edge = new EdgeNode();
            m_root_node->addChildNode(edge);
            edge->m_id = std::make_pair(city_id1, city_id2);
            edge->m_weight = weight;
            edge->m_position.x = (city_map[city_id1]->m_position.x + city_map[city_id2]->m_position.x)/2;
            edge->m_position.y = (city_map[city_id1]->m_position.y + city_map[city_id2]->m_position.y)/2;
            edge->m_position.z = -0.1;


            float deltaX = city_map[city_id1]->m_position.x - city_map[city_id2]->m_position.x;
            float deltaY = city_map[city_id1]->m_position.y - city_map[city_id2]->m_position.y;
            float distance = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

            if (distance > 15.0) {
                std::cout << "City: " << city_name1 << " " << city_name2 << " is over 15.0" << std::endl;
            }
            //Get scale based on distance
            edge->m_scale.x = distance;
            edge->m_scale.y = 0.025;

            //Get rotation based on the two cities
            edge->m_rotation = atan2(deltaY, deltaX);

            //Add to our edge map
            edge_map[edge->m_id] = edge;

            id++;
        }
    }
}

void SimScene::loadManifest() {
    //TODO: Load and parse a manifest file
}
