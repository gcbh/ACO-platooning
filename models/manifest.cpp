//
//  manifest.cpp
//  models
//
//  Created by Geoffrey Heath on 2018-01-11.
//  Copyright © 2018 University of Waterloo. All rights reserved.
//

#include "manifest.hpp"

manifest::manifest() {  }

void manifest::insert(int src, int dest, int duration) {
    pair<int, int> key = make_pair(src, dest);
    m.insert(make_pair(key, duration));
}

int manifest::count(int src, int dest) {
    return m.count(make_pair(src, dest));
}

int manifest::size() {
    return m.size();
}

multimap< pair<int, int> , int>::iterator manifest::begin() {
    return m.begin();
}

multimap< pair<int, int> , int>::iterator manifest::end() {
    return m.end();
}

void manifest::data(string line, manifest* m) {
    stringstream   linestream(line);
    int            src, dest, duration;
    
    linestream >> src >> dest >> duration;
    
    m->insert(src, dest, duration);
}
