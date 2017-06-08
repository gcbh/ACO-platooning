//
//  main.cpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2017-06-06.
//
//

#include <stdio.h>
#include "ACO.h"
#include <iostream>
#include <ifstream>
#include <map>

std::map<int, t_node> graph;

int main() {
    ifstream myReadFile;
    myReadFile.open("cities_p.txt");
    
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {
            myReadFile >> output;
            cout<<output;
            
        }
    }
    myReadFile.close();
    return 0;
}
