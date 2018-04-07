//
//  config.hpp
//  models
//
//  Created by Geoffrey Heath on 2018-01-11.
//  Copyright © 2018 University of Waterloo. All rights reserved.
//

#ifndef config_hpp
#define config_hpp

#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

class config {
public:
    config();
    config(string map, string distribution_cntr, string manifest, float i_a, float i_primer, float i_b, float i_d, float i_l, float i_p, float i_r, bool i_debug, int i_iters);
    string getMap();
    string getDistributionCenter();
    string getManifest();
    float getAlpha();
    float getPrimer();
    float getBeta();
    float getDelta();
    float getLambda();
    float getPhi();
    float getRho();
    bool DEBUG();
    int ITERS();
private:
    string map_name;
    string distribution_center_name;
    string manifest_name;
    float alpha;
    float primer;
    float beta;
    float delta;
    float lambda;
    float phi;
    float rho;
    bool debug;
    int iters;
};

#endif /* config_hpp */
