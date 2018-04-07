//
//  Heatmap.h
//  FreightSimulator
//
//  Created by Sean Amadio on 2018-02-05.
//  Copyright © 2018 FYDP. All rights reserved.
//

#ifndef Heatmap_h
#define Heatmap_h

#include <vector>
#include <iostream>
#include "lodepng.h"

using namespace std;

class Heatmap
{
public:
    static unsigned size;
    static std::vector<unsigned char> map;
    //-- Loads a default
    static void setup() {
        unsigned height;
        unsigned error = lodepng::decode(map, size, height, "colormap_jet.png");

        if(error != 0) {
            std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
        }
    }

    //-- Inputs a (value) between 0 and 1 and outputs the (red), (green) and (blue)
    //-- values representing that position in the gradient.
    static void getColorAtValue(const float value, float &red, float &green, float &blue) {
        int mappedValue = (int) (value*(float)(size-1));
        red   = (float)map[mappedValue*4] / 255.0;
        green = (float)map[mappedValue*4+1] / 255.0;
        blue  = (float)map[mappedValue*4+2] / 255.0;
        return;
    }
};

#endif /* Heatmap_h */
