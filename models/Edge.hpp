//
//  Edge.hpp
//  
//
//  Created by Sean Amadio on 2017-06-06.
//
//

#ifndef Edge_hpp
#define Edge_hpp

#include <stdio.h>
#include <string>

class Edge
{
    int id_1;
    int id_2;
    int distance;
    int speed_limit;

public:
    Edge(int id_1, int id_2, int distance, int speed_limit);
    Edge(std::string row);
    std::string getString();
    
};

#endif /* Edge_hpp */
