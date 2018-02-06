//
//  SimCamera.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-06.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef SimCamera_hpp
#define SimCamera_hpp

#include <stdio.h>
#include "Camera.hpp"

class SimCamera : public Camera
{
public:
    SimCamera() : Camera() {}

    void setup();
    void input(InputState* is);
    void update(UpdateState* us);

private:
};

#endif /* SimCamera_hpp */
