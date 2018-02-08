//
//  main.cpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2017-06-06.
//
//

#include "Galgo.hpp"

#include "ga_objective.hpp"
#include "config_factory.hpp"

#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Dijkstra.hpp"
#include "ACO.hpp"
#include "cost_function.hpp"
#include "output_extractor.hpp"
#include "graph_processor.hpp"

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
//void write_final_output(ACO* aco, graph* g, int num_vehicles, Dijkstra *dijkstra);

int main(int argc, const char * argv[]) {

    cout << "Begin optimization" << endl;
    
    config_factory conf_fac;
    config conf = conf_fac.build();
    
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
  
    delete gp;
    
    map_data* delete_map = ga_objective<double>::map();
    ga_objective<double>::map() = &gp_processed_map;
    delete delete_map;
    
    manifest* delete_man = ga_objective<double>::manifest_d();
    ga_objective<double>::manifest_d() = &manifest_map;
    delete delete_man;
    
    Dijkstra* delete_dj = ga_objective<double>::dijkstra();
    ga_objective<double>::dijkstra() = dijkstra;
    delete delete_dj;
    
    ga_objective<double>::num_iters() = conf.ITERS();
    
    // initializing parameters lower and upper bounds
    // an initial value can be added inside the initializer list after the upper bound
    galgo::Parameter<double> alpha({1.0, 10.0});
    galgo::Parameter<double> beta({1.0,10.0});
    galgo::Parameter<double> delta({0.0,10.0});
    galgo::Parameter<double> lambda({0.0,10.0});
    galgo::Parameter<double> phi({1.0,10.0});
    galgo::Parameter<double> rho({0.0f,1.0f});
    
    try {
        // initiliazing genetic algorithm
        galgo::GeneticAlgorithm<double> ga(ga_objective<double>::Objective,10,50,true,alpha, beta, delta, lambda, phi, rho);
        ga.run();
    } catch (exception& e) {
        e.what();
    }
    
    cout << "Run completed"<< endl;
    
//    delete dijkstra;
    
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

//void write_final_output(ACO* aco, graph* g, int num_vehicles) {
//    output_extractor* extractor = new output_extractor(g, num_vehicles);
//    extractor->extract_output(aco->result());
//
//    delete extractor;
//}
