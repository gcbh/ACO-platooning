
#ifndef graph_processor_hpp
#define graph_processor_hpp

#include <stdio.h>

#include "Dijkstra.hpp"

using namespace std;

class graph_processor {
public:
    graph_processor(Dijkstra* dj);
    virtual ~graph_processor(); 
private:
    Dijkstra*               dijkstra;
};

#endif