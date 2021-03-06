//
//  SimScene.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-05.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef SimScene_hpp
#define SimScene_hpp

#include <stdio.h>
#include <map>
#include <GLFW/glfw3.h>
#include "CityNode.hpp"
#include "TruckNode.hpp"
#include "EdgeNode.hpp"
#include "Scene.hpp"

class SimScene : public Scene
{
public:

    SimScene() : Scene() {}

    void postsetup();
    void preupdate(UpdateState* us);
    void prerender(RenderState* rs);
    void postrender(RenderState* rs);

    void loadGraph();
    void loadSchedule();
    void closestCity();

    static GLuint city_program, edge_program,
                  city_mvp_id, edge_mvp_id,
                  vbo, vao, tex;

    //Data
    static std::map<int, CityNode*> city_map;
    static std::map<int, TruckNode*> aco_truck_map;
    static std::map<int, TruckNode*> dijkstra_truck_map;
    static std::map<int, EdgeNode*> edge_map;

    //UI State
    static TruckNode* highlighted;
    static TruckNode* selected;

private:
    CityNode* addCityNode(int            city_id,
                     std::string    city_name,
                     float          latitude,
                     float          longitude);
    EdgeNode* addEdgeNode(int            city_id1,
                     int            city_id2,
                     float          weight);

    void clearStaticHeatMaps();
    void clearTruckMaps();

    void parseSchedule(json schedules,
                       std::map<int, TruckNode*> &truck_map,
                       std::map<int, float> &heat_map,
                       SceneNode* layer,
                       TruckType type);

    void updateTruckMap(std::map<int, TruckNode*> &truck_map,
                        float timecode);

    void renderUI(RenderState* rs);

    glm::vec3 camera_position;

    std::map<int, float> aco_static_heatmap;
    std::map<int, float> dijkstra_static_heatmap;

    json metadata;

    float sim_time_scale;
    float sim_max_time;
    bool reset_time_flag;

    bool show_view_window;
    bool show_info_window;
    bool show_trucks_window;
    bool show_schedule_window;
    bool show_playback_window;

    SceneNode* edgeLayer;
    SceneNode* cityLayer;
    SceneNode* acoLayer;
    SceneNode* dijkstraLayer;
};

#endif /* SimScene_hpp */
