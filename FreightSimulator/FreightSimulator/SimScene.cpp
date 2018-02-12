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
#include "StringUtils.hpp"
#include "glm.hpp"
#include "SimCamera.hpp"
#include "lodepng.h"
#include <math.h>
#include "json.hpp"
#include "tinyfiledialogs.h"

// Allow unordered hashing of pairs (for edge map)
int hash_pair(std::pair<int,int> p) {
    int a, b;

    // Sort
    a = fmin(p.first,p.second);
    b = fmax(p.first,p.second);

    return a + b * b;
}

// for convenience
using json = nlohmann::json;

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

ImVec2 prevMouseDrag = ImVec2();

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

    //UI Setup
    show_map_window = 1;
    show_schedule_window = 1;
    sim_time_scale = 0.0;
    sim_max_time = 100.0;
}

void SimScene::preupdate(UpdateState *us){
    double new_sim_time = us->sim_time + us->deltaTime * sim_time_scale;
    us->sim_time = fmax(0.0, fmin(new_sim_time, sim_max_time));

    updateTruckMap(aco_truck_map, us->sim_time);
    updateTruckMap(dijkstra_truck_map, us->sim_time);
}

void SimScene::prerender(RenderState* rs) {

    //Setup the heatmaps
    rs->aco_static_heatmap = &aco_static_heatmap;
    rs->dijkstra_static_heatmap = &dijkstra_static_heatmap;
    rs->aco_dynamic_heatmap = &aco_dynamic_heatmap;
    rs->dijkstra_dynamic_heatmap = &dijkstra_dynamic_heatmap;

    renderUI(rs);

    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("MainWindow", NULL, ImGui::GetIO().DisplaySize, 0.0f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoFocusOnAppearing|ImGuiWindowFlags_NoBringToFrontOnFocus);

    if(!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)){
        ImGuiIO& io = ImGui::GetIO();
        float mouseWheel = io.MouseWheel;
        m_scene_camera->m_zoom -= mouseWheel;

        ImVec2 mouseDrag = ImGui::GetMouseDragDelta(0, 0.0f);
        ImVec2 mouseDragDelta = ImVec2(mouseDrag.x - prevMouseDrag.x, mouseDrag.y - prevMouseDrag.y);

        if (!ImGui::IsMouseReleased(0)) {
            m_scene_camera->m_position.x -= mouseDragDelta.x/m_scene_camera->m_zoom;
            m_scene_camera->m_position.y += mouseDragDelta.y/m_scene_camera->m_zoom;
        }

        prevMouseDrag = mouseDrag;
    }
}

void SimScene::postrender(RenderState* rs) {
    ImGui::End(); //End main window
}

void SimScene::loadGraph() {
    char const * lTheOpenFileName;

    lTheOpenFileName = tinyfd_openFileDialog(
                                             "Choose Map File",
                                             "",
                                             0,
                                             NULL,
                                             NULL,
                                             0);

    if (! lTheOpenFileName) {
        printf("Error selecting map file");
        return;
    }

    //Load cities, also get a middle point to put the camera at to start
    std::ifstream cities_file(lTheOpenFileName);
    std::string   line;
    float         avgX = 0.0f;
    float         avgY = 0.0f;

    while(getline(cities_file, line)) {
        std::stringstream   linestream(line);
        int            city1_id;
        std::string    city1_name;
        float          city1_lat;
        float          city1_long;
        int            city2_id;
        std::string    city2_name;
        float          city2_lat;
        float          city2_long;
        float          weight;


        linestream >> city1_id >> city1_name >> city1_lat >> city1_long >> city2_id >> city2_name >> city2_lat >> city2_long >> weight;

        //Check the city list for both cities before creating the edge, create any missing ones
        if(city_map.find(city1_id) == city_map.end()) {
            addCityNode(city1_id, city1_name, city1_lat, city1_long);
            avgX += city1_long;
            avgY += city1_lat;
        }

        if(city_map.find(city2_id) == city_map.end()) {
            addCityNode(city2_id, city2_name, city2_lat, city2_long);
            avgX += city2_long;
            avgY += city2_lat;
        }

        addEdgeNode(city1_id, city2_id, weight);
    }

    avgX /= city_map.size();
    avgY /= city_map.size();
    m_scene_camera->m_position.x = avgX;
    m_scene_camera->m_position.y = avgY;
    m_scene_camera->m_focal_point.x = avgX;
    m_scene_camera->m_focal_point.y = avgY;
}

CityNode* SimScene::addCityNode(int city_id, std::string city_name, float latitude, float longitude) {
    CityNode* city = new CityNode();
    m_root_node->addChildNode(city);
    city->m_id = city_id;
    city->m_name = city_name;
    city->m_position.x = longitude;
    city->m_position.y = latitude;

    //Add to our city map
    city_map[city->m_id] = city;
    return city;
}

EdgeNode* SimScene::addEdgeNode(int city_id1, int city_id2, float weight) {

    EdgeNode* edge = new EdgeNode();
    m_root_node->addChildNode(edge);
    edge->m_id = hash_pair(std::make_pair(city_id1, city_id2));
    edge->m_weight = weight;
    edge->m_position.x = (city_map[city_id1]->m_position.x + city_map[city_id2]->m_position.x)/2;
    edge->m_position.y = (city_map[city_id1]->m_position.y + city_map[city_id2]->m_position.y)/2;
    edge->m_position.z = -0.1;
    edge->m_position_pair = std::make_pair(
                                           glm::vec4(city_map[city_id1]->m_position.x, city_map[city_id1]->m_position.y, 0.0f, 1.0f),
                                           glm::vec4(city_map[city_id2]->m_position.x, city_map[city_id2]->m_position.y, 0.0f, 1.0f)
                                           );
    edge->m_static_heat = 0.0f;
    edge->m_dynamic_heat = 0.0f;

    //Add to our edge map
    edge_map[edge->m_id] = edge;

    //Set default heat values
    aco_static_heatmap[edge->m_id] = 0.0f;
    dijkstra_static_heatmap[edge->m_id] = 0.0f;
    aco_dynamic_heatmap[edge->m_id] = 0.0f;
    dijkstra_dynamic_heatmap[edge->m_id] = 0.0f;
    return edge;
}

void SimScene::loadSchedule() {

    char const * lTheOpenFileName;

    lTheOpenFileName = tinyfd_openFileDialog(
                                             "Choose Schedule File",
                                             "",
                                             0,
                                             NULL,
                                             NULL,
                                             0);

    if (! lTheOpenFileName) {
        printf("Error selecting schedule file");
        return;
    }
    
    //Load and the schedule file
    std::ifstream schedule_file(lTheOpenFileName);

    json j;
    schedule_file >> j;

    // Pull the data from the file
    json metadata = j["metadata"];
    json schedules = j["schedules"];
    json dijkstra_schedules = j["dijkstra_schedule"];

    //Clear out old data
    clearStaticHeatMaps();
    clearTruckMaps();

    parseSchedule(schedules, aco_truck_map, aco_static_heatmap, TruckType::ACO);
    parseSchedule(dijkstra_schedules, dijkstra_truck_map, dijkstra_static_heatmap, TruckType::Dijkstra);
}

void SimScene::parseSchedule(json schedules,
                             std::map<int, TruckNode*> &truck_map,
                             std::map<int, float> &heat_map,
                             TruckType type) {
    int max_heat = 1;
    std::map<int, int> unnormalized_heat_map;
    for (auto& schedule : schedules) {
        TruckNode* t = new TruckNode(schedule);
        t->m_type = type;

        truck_map[t->m_id] = t;

        //Create temporary non-normalized heat map
        std::vector<Segment*>::iterator j;
        for (j = t->m_schedule.begin(); j != t->m_schedule.end(); j++) {
            Segment* s = *j;
            int edge_id = hash_pair(std::make_pair(s->start_node, s->end_node));

            unnormalized_heat_map[edge_id] += 1;
            max_heat = fmax(max_heat, unnormalized_heat_map[edge_id]);
        }

        //Add to master scene graph
        m_root_node->addChildNode(t);
    }

    //Assign normalized heat values
    std::map<int, int>::iterator k;
    for (k = unnormalized_heat_map.begin(); k != unnormalized_heat_map.end(); k++) {
        heat_map[k->first] = float(k->second) / float(max_heat);
    }
}

void SimScene::clearStaticHeatMaps() {
    aco_static_heatmap.clear();
    dijkstra_static_heatmap.clear();
}

void SimScene::clearTruckMaps() {
    aco_truck_map.clear();
    dijkstra_truck_map.clear();
}

void SimScene::updateTruckMap(std::map<int, TruckNode*> &truck_map, float timecode) {
    std::map<int, TruckNode*>::iterator i;
    for (i = truck_map.begin(); i != truck_map.end(); i++) {

        double time = 0.0;
        std::vector<Segment*>::iterator j;
        for (j = i->second->m_schedule.begin(); j != i->second->m_schedule.end(); j++) {
            Segment* s = *j;

            double segment_end_time = time + s->time;

            //We are on the current segment
            if (timecode <= segment_end_time) {
                double segment_progress = fmax(0.0, timecode - time) / s->time;

                float x = city_map[s->start_node]->m_position.x + (city_map[s->end_node]->m_position.x - city_map[s->start_node]->m_position.x)*segment_progress;
                float y = city_map[s->start_node]->m_position.y + (city_map[s->end_node]->m_position.y - city_map[s->start_node]->m_position.y)*segment_progress;
                i->second->m_position = glm::vec3(x,y,0.0f);
                break;
            }

            time = segment_end_time;
        }
    }
}

void SimScene::renderUI(RenderState* rs) {
    // Main Menu Bar
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open Map")) {
                loadGraph();
            }
            if (ImGui::MenuItem("Open Schedule")) {
                loadSchedule();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                exit(0);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Windows")) {
            ImGui::MenuItem("Map", NULL, &show_map_window);
            ImGui::MenuItem("Schedule", NULL, &show_schedule_window);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Render map window UI
    if (show_map_window) {
        if (!ImGui::Begin("Map")) {
            ImGui::End();
            return;
        }

        //Show map information
        if (ImGui::Button("Load Map")) {
            loadGraph();
        }

        if (ImGui::CollapsingHeader("View")) {

            ImGui::Text("City Mode");
            ImGui::RadioButton("None##CM", (int*)&rs->cityMode, 0); ImGui::SameLine();
            ImGui::RadioButton("Default##CM", (int*)&rs->cityMode, 1);

            ImGui::Text("City Label Mode");
            ImGui::RadioButton("None##CLM", (int*)&rs->cityLabelMode, 0); ImGui::SameLine();
            ImGui::RadioButton("Name##CLM", (int*)&rs->cityLabelMode, 1); ImGui::SameLine();
            ImGui::RadioButton("ID##CLM", (int*)&rs->cityLabelMode, 2); ImGui::SameLine();
            ImGui::RadioButton("Name & ID##CLM", (int*)&rs->cityLabelMode, 3);

            ImGui::Text("Road Mode");
            ImGui::RadioButton("None##RM", (int*)&rs->roadMode, 0); ImGui::SameLine();
            ImGui::RadioButton("Default##RM", (int*)&rs->roadMode, 1); ImGui::SameLine();
            ImGui::RadioButton("Static Heat##RM", (int*)&rs->roadMode, 2); ImGui::SameLine();
            ImGui::RadioButton("Dynamic Heat##RM", (int*)&rs->roadMode, 3);

            ImGui::Text("Road Label Mode");
            ImGui::RadioButton("None##RLM", (int*)&rs->roadLabelMode, 0); ImGui::SameLine();
            ImGui::RadioButton("Distance##RLM", (int*)&rs->roadLabelMode, 1); ImGui::SameLine();
            ImGui::RadioButton("Static Heat##RLM", (int*)&rs->roadLabelMode, 2); ImGui::SameLine();
            ImGui::RadioButton("Dynamic Heat##RLM", (int*)&rs->roadLabelMode, 3);
        }
        ImGui::End();
    }

    // Render schedule window UI
    if (show_schedule_window) {
        if (!ImGui::Begin("Schedule")) {
            ImGui::End();
            return;
        }

        //Show map information
        if (ImGui::Button("Load Schedule")) {
            loadSchedule();
        }

        //ImGui::DragFloat("Time Scale", &sim_time_scale, 0.01f, -2.0f, 2.0f, "%.06f x");

        if (ImGui::CollapsingHeader("Sim")) {
            ImGui::Text("Playback");
            if (ImGui::Button("<<<")) {
                sim_time_scale = -3.0;
            }
            ImGui::SameLine();
            if (ImGui::Button("<<")) {
                sim_time_scale = -2.0;
            }
            ImGui::SameLine();
            if (ImGui::Button("<")) {
                sim_time_scale = -1.0;
            }
            ImGui::SameLine();
            if (ImGui::Button("o")) {
                sim_time_scale = 0.0;
            }
            ImGui::SameLine();
            if (ImGui::Button(">")) {
                sim_time_scale = 1.0;
            }
            ImGui::SameLine();
            if (ImGui::Button(">>")) {
                sim_time_scale = 2.0;
            }
            ImGui::SameLine();
            if (ImGui::Button(">>>")) {
                sim_time_scale = 3.0;
            }
            ImGui::Text("Timecode: %f", rs->sim_time);
        }

        if (ImGui::CollapsingHeader("View")) {
            ImGui::Text("Truck Mode");
            ImGui::RadioButton("None##RLM", (int*)&rs->truckMode, 0); ImGui::SameLine();
            ImGui::RadioButton("Dijkstra##RLM", (int*)&rs->truckMode, 1); ImGui::SameLine();
            ImGui::RadioButton("ACO##RLM", (int*)&rs->truckMode, 2); ImGui::SameLine();
            ImGui::RadioButton("Dijkstra & ACO##RLM", (int*)&rs->truckMode, 3);
        }

        // Show node list
//        if (ImGui::CollapsingHeader("Truck")) {
//            if (truck_map.size() > 0) {
//                std::map<int, TruckNode*>::iterator i;
//                for (i = truck_map.begin(); i != truck_map.end(); i++) {
//                    if (ImGui::TreeNode(i->second, "%d", i->second->m_id)) {
//                        std::vector<Segment*>::iterator j;
//                        for (j = i->second->m_schedule.begin(); j != i->second->m_schedule.end(); j++) {
//                            Segment* s = *j;
//                            ImGui::Text("%s -> %s", city_map[s->start_node]->m_name.c_str(), city_map[s->end_node]->m_name.c_str());
//                        }
//                        ImGui::TreePop();
//                    }
//
//                }
//            } else {
//                ImGui::Text("No schedule loaded");
//            }
//        }
        ImGui::End();
    }
}
