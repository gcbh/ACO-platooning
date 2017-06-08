//
//  Edge.cpp
//  
//
//  Created by Sean Amadio on 2017-06-06.
//
//

#include "Edge.hpp"
#include "../utils/StringUtils.hpp"
#include <sstream>

Edge::Edge(int id_1, int id_2, int distance, int speed_limit)
{
    id_1 = id_1;
    id_2 = id_2;
    distance = distance;
    speed_limit = speed_limit;
}

Edge::Edge(std::string s)
{
    std::vector<std::string> args = split(s, ',');
    id_1 = std::stoi(args[0]);
    id_2 = std::stoi(args[1]);
    distance = std::stoi(args[2]);
    speed_limit = std::stoi(args[3]);
}

std::string Edge::getString()
{
    std::ostringstream oss;
    oss << id_1 << "," << id_2 << "," << distance << "," << speed_limit;
    return oss.str();
}
