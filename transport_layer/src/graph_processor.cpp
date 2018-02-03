
#include "graph_processor.hpp"

graph_processor::graph_processor(Dijkstra* dj) {
    dj = dijkstra;
    distribution_nodes = new map<iPair, string>();
}

graph_processor::~graph_processor() {
    delete [] dijkstra;
}

void graph_processor::get_distribution_nodes(string distribution_center) {
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
}
