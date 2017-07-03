//
//  StringUtils.hpp
//  transport_layer
//
//  Created by Aditi Lath on 2017-07-03.
//
//

#ifndef StringUtils_hpp
#define StringUtils_hpp

#include <stdio.h>
#include <vector>

using namespace std;

class StringUtils {
public:
    vector<string> split(const string &s, char delim);
};

#endif /* StringUtils_hpp */
