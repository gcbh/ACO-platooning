//
//  config_factory.cpp
//  utils
//
//  Created by Geoffrey Heath on 2018-01-11.
//  Copyright © 2018 University of Waterloo. All rights reserved.
//

#include "config_factory.hpp"

#define CONFIG_FILE "../application.conf"

config_factory::config_factory() {  }

config config_factory::build() {
    config_data d;
    ifstream file(CONFIG_FILE);
    string   line;
    
    while (getline(file, line)) {
        stringstream linestream(line);
        string key;
        string value;
        
        linestream >> key >> value;
        convert(&d, key, value);
    }
    
    // Instantiation of immutable config data structure
    config c(d.map_name, d.distribution_center_name, d.manifest_name, d.alpha, d.primer, d.beta, d.delta, d.lambda, d.phi, d.rho, d.debug, d.iters);
    return c;
}

// Mapping of key to data structure location
void config_factory::convert(config_data* d, string key, string value) {
    if (key == "map") {
        d->map_name = value;
    } if (key == "distribution") {
        d->distribution_center_name = value;
    } else if (key == "manifest") {
        d->manifest_name = value;
    } else if (key == "alpha") {
        d->alpha = atof(value.c_str());
    } else if (key == "primer") {
        d->primer = atof(value.c_str());
    } else if (key == "beta") {
        d->beta = atof(value.c_str());
    } else if (key == "delta") {
        d->delta = atof(value.c_str());
    } else if (key == "lambda") {
        d->lambda = atof(value.c_str());
    } else if (key == "phi") {
        d->phi = atof(value.c_str());
    } else if (key == "rho") {
        d->rho = atof(value.c_str());
    } else if (key == "debug") {
        d->debug = (atoi(value.c_str()) == 1) ? true : false;
    } else if (key == "iters") {
        d->iters = atoi(value.c_str());
    } else if (key == "dist_center") {
        d->distribution_center_name = value;
    }
}

