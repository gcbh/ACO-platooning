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
#include <map>
#include "SceneNode.hpp"
#include "State.hpp"
#include "glm.hpp"
#include "Camera.hpp"

class SceneNode;
class Scene
{
public:
    Scene();
    ~Scene();

    //Public overrideable
    virtual void presetup() {};
    virtual void postsetup() {};
    virtual void preinput(InputState is) {};
    virtual void postinput(InputState is) {};
    virtual void preupdate(UpdateState us) {};
    virtual void postupdate(UpdateState us) {};
    virtual void prerender(RenderState rs) {};
    virtual void postrender(RenderState rs) {};

    //Internal final
    void _setup();
    void _input(InputState is);
    void _update(UpdateState us);
    void _render(RenderState rs);

    //Internal overrideable
    virtual void setup() {};
    virtual void input(InputState rs) {};
    virtual void update(UpdateState rs) {};
    virtual void render(RenderState rs) {};

    void attachSceneCamera(Camera* camera);

    void addTexture(std::string file_name);
    GLuint getTexture(std::string file_name);

    SceneNode* m_root_node;
    Camera* m_scene_camera;

private:
    std::map<std::string, GLuint> texture_map;
};

#endif /* Scene_hpp */
