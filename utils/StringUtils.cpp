//
//  StringUtils.cpp
//  
//
//  Created by Sean Amadio on 2017-06-06.
//
//

#include "StringUtils.hpp"

using namespace std;

vector<string> split(const string &s, char delim)
{
    stringstream ss(s);
    string item;
    vector<string> elems;
    
    while (getline(ss, item, delim)) {
        if (!item.empty())
            elems.push_back(item);
    }
    return elems;
}

std::string readFile(const std::string file_path) {
    std::ifstream file_stream(file_path);
    std::string str((std::istreambuf_iterator<char>(file_stream)),
                    std::istreambuf_iterator<char>());
    return str;
}
