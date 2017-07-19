//
//  Camera.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-06.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include "State.hpp"

class Camera
{
public:
    Camera();
    ~Camera();

    virtual void setup() {};
    virtual void input(InputState is) {};
    virtual void update(UpdateState us) {};

    void _setup();
    void _input(InputState is);
    void _update(UpdateState us);

    glm::mat4 vp_matrix;
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    glm::vec3 m_focal_point;
    float m_width;
    float m_height;
    float m_zoom;

private:

};

#endif /* Camera_hpp */
