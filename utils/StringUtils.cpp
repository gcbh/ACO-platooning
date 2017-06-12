//
//  StringUtils.cpp
//  
//
//  Created by Sean Amadio on 2017-06-06.
//
//

#include "StringUtils.hpp"
#include <fstream>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

std::string readFile(const std::string file_path) {
    std::ifstream file_stream(file_path);
    std::string str((std::istreambuf_iterator<char>(file_stream)),
                    std::istreambuf_iterator<char>());
    return str;
}
