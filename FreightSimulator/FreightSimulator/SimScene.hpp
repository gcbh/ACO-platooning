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
#include "EdgeNode.hpp"
#include "Scene.hpp"

class SimScene : public Scene
{
public:

    SimScene() : Scene() {}

    void setup();
    void input(InputState is);
    void update(UpdateState is);
    void render(RenderState rs);

    static GLuint city_program, edge_program,
                  city_mvp_id, edge_mvp_id,
                  vbo, vao, tex;

private:
    glm::vec3 camera_position;
    CityNode* camera_city;
    std::map<int, CityNode*> city_map;
    std::map<std::pair<int,int>, EdgeNode*> edge_map;
};

#endif /* SimScene_hpp */
