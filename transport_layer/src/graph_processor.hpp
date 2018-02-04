
#ifndef graph_processor_hpp
#define graph_processor_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <iterator>

#include "../../utils/StringUtils.hpp"
#include "../../models/map_data.hpp"
#include "Dijkstra.hpp"
#include "../../models/map_data.hpp"

using namespace std;
typedef pair<int, int> iPair;

class graph_processor {
public:
    graph_processor();
    virtual                 ~graph_processor();
    map<iPair, string>*     distribution_nodes;
    map<iPair, string>*     get_distribution_nodes(string distribution_center);
    void                    update_distribution_nodes(iPair edge, string route);
    int                     distribution_nodes_count(iPair edge);
    map_data                format_distribution_graph(Dijkstra* dj);
    void                    create_distribution_map_file(map_data map, string file);
private:

};

#endif
