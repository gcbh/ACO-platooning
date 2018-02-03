
#ifndef graph_processor_hpp
#define graph_processor_hpp

#include <stdio.h>

#include "Dijkstra.hpp"

using namespace std;
typedef pair<int, int> iPair;

class graph_processor {
public:
    graph_processor(Dijkstra* dj);
    virtual                 ~graph_processor();
    map<iPair, string>*     distribution_nodes;
    void                    get_distribution_nodes(string distribution_center);
private:
    Dijkstra*               dijkstra;
};

#endif
