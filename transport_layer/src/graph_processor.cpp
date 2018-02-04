
#include "graph_processor.hpp"

graph_processor::graph_processor() {
    distribution_nodes = new map<iPair, string>();
}

graph_processor::~graph_processor() {
    //delete distribution nodes
}

map<iPair, string>* graph_processor::get_distribution_nodes(string distribution_center) {
    stringstream ss;
    ss << "../../maps/distribution_centers/" << distribution_center;
    ifstream file(ss.str());
    string line;
    getline(file, line);
    vector<string> columns;
    list<int>* nodes = new list<int>();

    while(getline(file, line)) {
        columns = split(line, ' ');
        nodes->push_back(stoi(columns[0]));
    }

    for (list<int>::iterator it = nodes->begin(); it != nodes->end(); ++it) {
        for (list<int>::iterator it2 = it; it2 != nodes->end(); ++it2) {
            if (it2 != it) {
                distribution_nodes->insert(make_pair(make_pair((*it), (*it2)), ""));
            }
        }
    }

    return distribution_nodes;
}

void graph_processor::update_distribution_nodes(iPair edge, string route) {
    (*distribution_nodes)[edge] = route;
}

int graph_processor::distribution_nodes_count(iPair edge) {
    return distribution_nodes->count(edge);
}

map_data graph_processor::format_distribution_graph(Dijkstra* dj) {
    map_data graph_map;
    
    for (map<pair<int, int>, string>::iterator it = distribution_nodes->begin(); it != distribution_nodes->end(); ++it) {
        vector<string> route = split(it->second, ' ');

        int src = stoi(route[0]);
        for (vector<string>::iterator it2 = route.begin(); ++it2 != route.end();) {
            int dest = stoi(*it2);
            int weight = dj->get_edge_weight(src, dest);
            graph_map.insert(src, dest, weight);
        }
    }

    return graph_map;
}


