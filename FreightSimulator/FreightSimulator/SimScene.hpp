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
#include <GLFW/glfw3.h>
#include "Scene.hpp"

class SimScene : public Scene
{
public:

    SimScene() : Scene() {}

    void setup();
    void input(InputState is);
    void update(UpdateState is);
    void render(RenderState rs);

    static GLuint program, vbo, vao, mvp_id;

private:

    glm::vec3 camera_position;
};

#endif /* SimScene_hpp */
