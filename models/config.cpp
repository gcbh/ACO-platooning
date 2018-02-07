//
//  config.cpp
//  models
//
//  Created by Geoffrey Heath on 2018-01-11.
//  Copyright © 2018 University of Waterloo. All rights reserved.
//

#include "config.hpp"
config::config() { }
config::config(string map, string distribution_cntr, string manifest,float i_a, float i_b, float i_d, float i_l, float i_p, float i_r, bool i_debug, int i_iters) {
    map_name = map;
    distribution_center_name = distribution_cntr;
    manifest_name = manifest;
    alpha = i_a;
    beta = i_b;
    delta = i_d;
    lambda = i_l;
    phi = i_p;
    rho = i_r;
    debug = i_debug;
    iters = i_iters;
}

string config::getMap() {   return map_name;    }
string config::getDistributionCenter() {    return distribution_center_name;    }
string config::getManifest() {  return manifest_name;   }
float config::getAlpha() {  return alpha;   }
float config::getBeta() {   return beta;    }
float config::getDelta() {  return delta;   }
float config::getLambda() { return lambda;  }
float config::getPhi() {    return phi;     }
float config::getRho() {    return rho;     }
bool config::DEBUG() {  return debug;   }
int config::ITERS() {   return iters;   }
