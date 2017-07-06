//
//  App.hpp
//  FreightSimulator
//
//  https://placeofmemory.wordpress.com/2013/11/24/a-very-simple-glfw-project-framework/
//
//  Created by Sean Amadio on 2017-06-08.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef App_hpp
#define App_hpp

#include <stdio.h>
#include <string>
#include <GLFW/glfw3.h>
#include "State.hpp"

class App
{
public:
    App() {}
    virtual std::string getName() = 0;
    virtual void setup() = 0;
    virtual void cleanup() = 0;
    virtual void input(InputState is) = 0;
    virtual void update(UpdateState us) = 0;
    virtual void render(RenderState rs) = 0;
};

#endif /* App_hpp */
