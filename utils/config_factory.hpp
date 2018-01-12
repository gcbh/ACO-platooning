//
//  config_factory.hpp
//  utils
//
//  Created by Geoffrey Heath on 2018-01-11.
//  Copyright © 2018 University of Waterloo. All rights reserved.
//

#ifndef config_factory_hpp
#define config_factory_hpp

#include <stdio.h>
#include "../models/config.hpp"

struct config_data {
    std::string map_name;
    std::string manifest_name;
    float alpha;
    float beta;
    float delta;
    float lambda;
    float phi;
    float rho;
    bool debug;
    int iters;
};

class config_factory {
public:
    config_factory();
    config build();
private:
    void convert(config_data* d, std::string key, std::string value);
};
#endif /* config_factory_hpp */
