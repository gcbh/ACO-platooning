//
//  main.cpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2017-06-06.
//
//

#include <stdio.h>
#include <time.h>
#include <list>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include "Dijkstra.hpp"
#include "ACO_new.hpp"
#include "../../models/graph.hpp"
#include "../../utils/StringUtils.hpp"

#define MAPS "../../maps/"
#define DJ_MAPS "../../maps/d_maps/"
#define MANIFESTS "../../manifests/"


using namespace std;

multimap< pair<int, int> , int>  get_manifest(string file_name);

int main(int argc, const char * argv[]) {
    if (argc <= 1) {
        cout << "Missing filename arguments" << endl;
        return -1;
    }
    
    cout << "Begin optimization" << endl;

    // algorithm optimization values
    float   ALPHA = 1.2f;       // pheromone exponent of heuristic
    float   BETA = 0.3f;        // dijkstra exponent of heuristic
    float   DELTA = 1.0f;       // pheromone incremental value
    float   LAMBDA = 3.0f;      // reinforcement pheromone value
    float   PHI = 1.2f;         // exploration coefficient
    float   RHO = 0.3f;         // pheromone evaporation value
    int     num_iterations = 100000;
    
    bool    DEBUG = true;
    
    time_t seed = (long)time(nullptr);
    
    string file_name = argv[1];
    string manifest_file_name = argv[2];
//    string file_name = "cities_p.txt";
//    string manifest_file_name = "manifest_1_cities_graph.txt";
//    string file_name = "small_graph.txt";
//    string manifest_file_name = "manifest_small_graph.txt";
//    string file_name = "triangular_graph.txt";
//    string manifest_file_name = "manifest_triangular_graph.txt";
    
    // open graph file, read and pass data to Dijkstra to calculate shortest path
    ifstream file(MAPS + file_name);
    string   line;
    
    Dijkstra *dijkstra = new Dijkstra();
    list<graph_data> pre_opt_graph;
    set<int> nodes;

    // Creates d_maps folder if it doesn't exist
    system(("mkdir -p " + string(DJ_MAPS)).c_str());

    while(getline(file, line))
    {
        stringstream   linestream(line);
        string         data;
        int            src;
        int            dest;
        string         src_name;
        string         dest_name;
        int            distance;

        linestream >> src >> src_name >> dest >> dest_name >> distance;
        
        struct graph_data edge;
        edge.src = src;
        edge.dest = dest;
        edge.weight = distance;
        
        nodes.insert(src);
        nodes.insert(dest);
        
        pre_opt_graph.push_back(edge);
    }
    
    string dijkstra_file_path = DJ_MAPS + ("dj_" + file_name);
    ifstream djfile(dijkstra_file_path);

    // check if dijkstra file exists, if not create one
    if (djfile.fail()) {

        // create file to output from dijkstra algorithm
        ofstream dijkstra_file;
        dijkstra_file.open(dijkstra_file_path, ios_base::out);
        
        // freopen to be able to write output to file directly
        freopen(dijkstra_file_path.c_str(), "w", stdout);
        dijkstra->init(pre_opt_graph, nodes.size());
        fclose(stdout);
        dijkstra_file.close();
    }
    
    multimap< pair<int, int> , int>  manifest_map = get_manifest(manifest_file_name);
    dijkstra->populate_from_dijkstra_file(dijkstra_file_path, manifest_map);
    
    graph *g = new graph();

    g->construct_graph(pre_opt_graph);

    ACO_new *ACO = new ACO_new(g, manifest_map, ALPHA, BETA, DELTA, LAMBDA, PHI, RHO, DEBUG, seed);
    
    ACO->init(dijkstra);
    
    for(int i = 1; i <= num_iterations; i++) {
        int cost = ACO->iteration();
        if (cost < 0) continue;
    }
    
    cout << "Run completed." << endl;

    delete g;
    delete dijkstra;
    delete ACO;

    return 0;
}

multimap< pair<int, int> , int> get_manifest(string file_name) {
    ifstream file(MANIFESTS+ file_name);
    string line;
    multimap< pair<int, int>, int> manifest_data;

    while(getline(file, line))
    {
        stringstream   linestream(line);
        string         data;
        int            src;
        int            dest;
        int            duration;
        
        linestream >> src >> dest >> duration;

        struct manifest_data edge;
        edge.src = src;
        edge.dest = dest;
        edge.duration = duration;

        pair<int, int> key = make_pair(src, dest);
        manifest_data.insert(make_pair(key, duration));
    }
    return manifest_data;
}
