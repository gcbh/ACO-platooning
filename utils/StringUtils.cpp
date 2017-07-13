//
//  StringUtils.cpp
//  
//
//  Created by Sean Amadio on 2017-06-06.
//
//

#include "StringUtils.hpp"
#include <fstream>

std::string readFile(const std::string file_path) {
    std::ifstream file_stream(file_path);
    std::string str((std::istreambuf_iterator<char>(file_stream)),
                    std::istreambuf_iterator<char>());
    return str;
}
