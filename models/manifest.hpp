//
//  manifest.hpp
//  models
//
//  Created by Geoffrey Heath on 2018-01-11.
//  Copyright © 2018 University of Waterloo. All rights reserved.
//

#ifndef manifest_hpp
#define manifest_hpp

#include <stdio.h>
#include <map>

using namespace std;

class manifest {
public:
    manifest();
    void insert(int src, int dest, int duration);
    int count(int src, int dest);
    int size();
    multimap< pair<int, int> , int>::iterator begin();
    multimap< pair<int, int> , int>::iterator end();
private:
    multimap< std::pair<int, int> , int> m;
};

#endif /* manifest_hpp */
