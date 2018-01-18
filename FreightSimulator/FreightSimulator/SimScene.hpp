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

    void postsetup();
    void preinput(InputState is);
    void preupdate(UpdateState us);
    void postupdate(UpdateState us);
    void prerender(RenderState rs);

    void loadGraph();
    void loadManifest();
    void closestCity();

    static GLuint city_program, edge_program,
                  city_mvp_id, edge_mvp_id,
                  vbo, vao, tex;

private:
    glm::vec3 camera_position;
    std::map<int, CityNode*> city_map;
    std::map<std::pair<int,int>, EdgeNode*> edge_map;
};

#endif /* SimScene_hpp */
