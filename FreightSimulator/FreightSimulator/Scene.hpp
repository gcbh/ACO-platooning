//
//  Scene.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-06-12.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include "SceneNode.hpp"
#include "State.hpp"
#include "glm.hpp"
#include "Camera.hpp"

class Scene
{
public:
    Scene();
    ~Scene();

    virtual void setup() = 0;
    virtual void input(InputState is) = 0;
    virtual void update(UpdateState us) = 0;
    virtual void render(RenderState rs) = 0;

    void _setup();
    void _input(InputState is);
    void _update(UpdateState us);
    void _render(RenderState rs);

    void attachSceneCamera(Camera* camera);

    SceneNode* m_root_node;


private:
    Camera* m_scene_camera;
};

#endif /* Scene_hpp */
