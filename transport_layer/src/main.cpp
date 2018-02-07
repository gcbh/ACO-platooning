//
//  main.cpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2017-06-06.
//
//

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Dijkstra.hpp"
#include "ACO.hpp"
#include "cost_function.hpp"
#include "output_extractor.hpp"
#include "graph_processor.hpp"

#include "../../utils/config_factory.hpp"
#include "../../models/config.hpp"
#include "../../models/map_data.hpp"
#include "../../models/manifest.hpp"
#include "../../models/graph.hpp"

#define MAPS "../../maps/"
#define DJ_MAPS "../../maps/d_maps/"
#define DISTRIBUTION_MAPS "../../maps/distribution_centers/"
#define MANIFESTS "../../manifests/"
#define SCRIPT "../../utils/scripts/"
#define SCRIPT_NAME "process_cities_complete.py"
#define COORDS_FILENAME "cities_ids_coords.txt"

using namespace std;

map_data get_data(string file_name);
map_data get_dist_data(string file_name);
manifest get_manifest(string file_name);
void write_dijkstras(Dijkstra* dijkstra, string file_path, map_data map);
void write_final_output(ACO* aco, graph* g, int num_vehicles);

int main(int argc, const char * argv[]) {

    cout << "Begin optimization" << endl;
    
    config_factory conf_fac;
    config conf = conf_fac.build();
    
    logger std_out;
    logger cost_out("../logs/cost.log", false);
    logger debug_log("../logs/debug.log", false);
    
    time_t seed = (long)time(nullptr);
    
    map_data cities_map = get_data(conf.getMap());
    manifest manifest_map = get_manifest(conf.getManifest());
    
    // Creates d_maps folder if it doesn't exist
    system(("mkdir -p " + string(DJ_MAPS)).c_str());

    string dijkstra_file_path = DJ_MAPS + ("dj_" + conf.getMap());
    ifstream djfile(dijkstra_file_path);

    Dijkstra *dijkstra = new Dijkstra();
    
    // check if dijkstra file exists, if not create one
    if (djfile.fail()) {
        write_dijkstras(dijkstra, dijkstra_file_path, cities_map);
    }

    graph *g = new graph();
    graph_processor *gp = new graph_processor();
    
    // process distribution center info
    string distr_cntr_file_path = DISTRIBUTION_MAPS + ("comp_gp_" + conf.getDistributionCenter());
    ifstream distrFile(distr_cntr_file_path);

    map< pair<int, int>, string>* gp_map = new map< pair<int, int>, string>();
    map_data gp_processed_map;
    
    if (distrFile.fail()) {
        gp_map = gp->get_distribution_nodes(DISTRIBUTION_MAPS + conf.getDistributionCenter());
        // Populate info from manifest and for graph processor
        dijkstra->populate_from_dijkstra_file(dijkstra_file_path, manifest_map, gp_map);
        
        gp_processed_map = gp->format_distribution_graph(dijkstra);
        
        string temp_file_path = DISTRIBUTION_MAPS + ("gp_" + conf.getDistributionCenter());
        gp->create_distribution_map_file(gp_processed_map, temp_file_path);

        // create file for Simulation
        system(("python " + string(SCRIPT) + SCRIPT_NAME + " " + temp_file_path + " "+ MAPS + COORDS_FILENAME).c_str());

        // delete the temp file
        system(("rm -f " + temp_file_path).c_str());

    } else {
        // Populate info for map
        dijkstra->populate_from_dijkstra_file(dijkstra_file_path, manifest_map, gp_map);
        gp_processed_map = get_dist_data(distr_cntr_file_path);
    }

    g->construct_graph(gp_processed_map);
  
    delete gp;

    
    
    heuristic_selector* sel = new heuristic_selector(conf.getAlpha(), conf.getBeta(), conf.getPhi(), seed, dijkstra);
    
    cost_function* cost = new cost_function();

    ACO *aco = new ACO(g, manifest_map, conf, sel, cost, std_out, cost_out, debug_log);
    aco->init(dijkstra);
    
    try {
        for(int i = 1; i <= conf.ITERS(); i++) {
            int cost = aco->iteration();
            if (cost < 0) continue;
        }
        
        // generate final output
        write_final_output(aco, g, manifest_map.size());

        delete g;
        delete dijkstra;
        delete sel;
        delete aco;
        
    } catch (const exception &e) {
        cout << e.what() << endl;
    }
    
    cout << "Run completed"<< endl;
    
    return 0;
}

void build_loggers() {
    
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

map_data get_dist_data(string file_name) {
    // open graph file, read and pass data to Dijkstra to calculate shortest path
    ifstream file(file_name);
    string   line;
    
    map_data map;
    
    while(getline(file, line))
    {
        stringstream   linestream(line);
        int            src;
        string         src_name;
        double         src_lat;
        double         src_long;
        int            dest;
        string         dest_name;
        double         dest_lat;
        double         dest_long;
        int            distance;
        
        linestream >> src >> src_name >> src_lat >> src_long >> dest >> dest_name >> dest_lat >> dest_long >> distance;
        
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

void write_final_output(ACO* aco, graph* g, int num_vehicles) {
    output_extractor* extractor = new output_extractor(g, num_vehicles);
    extractor->extract_output(aco->result());
    
    delete extractor;
}
