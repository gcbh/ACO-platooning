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
#include <fstream>
#include <sstream>
#include "Dijkstra.hpp"
#include "ACO.hpp"
#include "../../utils/config_factory.hpp"
#include "../../models/manifest.hpp"
#include "../../models/config.hpp"
#include "../../models/map_data.hpp"
#include "../../models/graph.hpp"
#include "../../utils/StringUtils.hpp"

#define MAPS "../../maps/"
#define DJ_MAPS "../../maps/d_maps/"
#define MANIFESTS "../../manifests/"

using namespace std;

map_data get_data(string file_name);
manifest get_manifest(string file_name);
void write_dijkstras(Dijkstra* dijkstra, string file_path, map_data map);

int main(int argc, const char * argv[]) {

    cout << "Begin optimization" << endl;
    
    config_factory conf_fac;
    config conf = conf_fac.build();
    
    time_t seed = (long)time(nullptr);
    
    Dijkstra *dijkstra = new Dijkstra();
    
    map_data map = get_data(conf.getMap());
    
    // Creates d_maps folder if it doesn't exist
    system(("mkdir -p " + string(DJ_MAPS)).c_str());

    string dijkstra_file_path = DJ_MAPS + ("dj_" + conf.getMap());
    ifstream djfile(dijkstra_file_path);

    // check if dijkstra file exists, if not create one
    if (djfile.fail()) {
        write_dijkstras(dijkstra, dijkstra_file_path, map);
    }
    
    manifest manifest_map = get_manifest(conf.getManifest());
    dijkstra->populate_from_dijkstra_file(dijkstra_file_path, manifest_map);
    
    graph *g = new graph();
    g->construct_graph(map);

    ACO *aco = new ACO(g, manifest_map, conf, seed);
    aco->init(dijkstra);
    
    for(int i = 1; i <= conf.ITERS(); i++) {
        int cost = aco->iteration();
        if (cost < 0) continue;
    }
    
    cout << "Run completed." << endl;

    delete g;
    delete dijkstra;
    delete aco;

    return 0;
}

map_data get_data(string file_name) {
    // open graph file, read and pass data to Dijkstra to calculate shortest path
    ifstream file(MAPS + file_name);
    string   line;
    
    map_data map;
    
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
        
        map.insert(src, dest, distance);
    }
    
    return map;
}

manifest get_manifest(string file_name) {
    ifstream file(MANIFESTS + file_name);
    string line;
    manifest manifest_data;

    while(getline(file, line))
    {
        stringstream   linestream(line);
        string         data;
        int            src;
        int            dest;
        int            duration;
        
        linestream >> src >> dest >> duration;

        manifest_data.insert(src, dest, duration);
    }
    
    return manifest_data;
}

void write_dijkstras(Dijkstra* dijkstra, string file_path, map_data map) {
    // create file to output from dijkstra algorithm
    ofstream dijkstra_file;
    dijkstra_file.open(file_path, ios_base::out);
    
    // freopen to be able to write output to file directly
    freopen(file_path.c_str(), "w", stdout);
    dijkstra->init(map);
    fclose(stdout);
    dijkstra_file.close();
}
