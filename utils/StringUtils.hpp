//
//  StringUtils.hpp
//
//  Created by Aditi Lath on 2017-07-03.
//
//

#ifndef StringUtils_hpp
#define StringUtils_hpp

#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <iterator>

using namespace std;

vector<string> split(const string &s, char delim);
std::string readFile(const std::string file_path);
std::string space(int size);

#endif /* StringUtils_hpp */
