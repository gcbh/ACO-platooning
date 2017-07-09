//
//  StringUtils.cpp
//
//  Created by Aditi Lath on 2017-07-03.
//
//

#include "StringUtils.hpp"

#include <stdio.h>
#include <fstream>
#include <sstream>

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
