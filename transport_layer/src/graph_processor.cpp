
#include "graph_processor.hpp"

graph_processor::graph_processor(Dijkstra* dj) {
    dj = dijkstra;
}

graph_processor::~graph_processor() {
    delete [] dijkstra;
}