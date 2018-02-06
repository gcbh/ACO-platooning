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

private:
    CityNode* addCityNode(int            city_id,
                     std::string    city_name,
                     float          latitude,
                     float          longitude);
    EdgeNode* addEdgeNode(int            city_id1,
                     int            city_id2,
                     float          weight);
    void generateStaticHeatMap();
    void clearStaticHeatMap();
    void positionTrucks(UpdateState* us);

    void renderUI(RenderState* rs);

    glm::vec3 camera_position;
    std::map<int, CityNode*> city_map;
    std::map<int, TruckNode*> truck_map;
    std::map<int, EdgeNode*> edge_map;

    float sim_time_scale;
    float sim_max_time;

    bool show_map_window;
    bool show_schedule_window;
};

#endif /* SimScene_hpp */
