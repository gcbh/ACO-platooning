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
#include "file_parser.hpp"

#define MAPS "../../maps/"
#define DJ_MAPS "../../maps/d_maps/"
#define DISTRIBUTION_MAPS "../../maps/distribution_centers/"
#define MANIFESTS "../../manifests/"
#define SCRIPT "../../utils/scripts/"
#define SCRIPT_NAME "process_cities_complete.py"
#define COORDS_FILENAME "cities_ids_coords.txt"

using namespace std;

void get_data(string line, map_data* m);
void get_dist_data(string line, map_data* m);
void get_manifest(string line, manifest* m);
galgo::GeneticAlgorithm<double> build_ga(config c, map_data* map, manifest* man, Dijkstra* d);

int main(int argc, const char * argv[]) {

    cout << "Begin optimization" << endl;
    
    config_factory conf_fac;
    config conf = conf_fac.build();
    
    map_data cities_map = file_parser::parse<map_data>(map_data::data, string(MAPS) + conf.getMap());
    manifest manifest_map = file_parser::parse<manifest>(manifest::data, string(MANIFESTS) + conf.getManifest());
    
    // Creates d_maps folder if it doesn't exist
    system(("mkdir -p " + string(DJ_MAPS)).c_str());

    map< pair<int, int>, string>* gp_map = new map< pair<int, int>, string>();
    Dijkstra *dijkstra = new Dijkstra();
    string dijkstra_file_path = DJ_MAPS + ("dj_" + conf.getMap());
    dijkstra->init(cities_map, dijkstra_file_path);
    
    if (conf.getDistributionCenter() != "") {
        graph_processor *gp = new graph_processor();
        
        // process distribution center info
        string distr_cntr_file_path = DISTRIBUTION_MAPS + ("comp_gp_" + conf.getDistributionCenter());
        ifstream distrFile(distr_cntr_file_path);

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
            gp_processed_map = file_parser::parse<map_data>(map_data::dist_data, distr_cntr_file_path);
        }
      
        cities_map = gp_processed_map;
        delete gp;
    } else {
        dijkstra->populate_from_dijkstra_file(dijkstra_file_path, manifest_map, gp_map);
    }
    
    try {
        // initiliazing genetic algorithm
        galgo::GeneticAlgorithm<double> ga = build_ga(conf, &cities_map, &manifest_map, dijkstra);
        ga.run();
    } catch (exception& e) {
        e.what();
    }
    
    cout << "Run completed"<< endl;
    
//    delete dijkstra;
    
    return 0;
}

galgo::GeneticAlgorithm<double> build_ga(config c, map_data* map, manifest* man, Dijkstra* d) {
    manifest* delete_man = ga_objective<double>::manifest_d();
    ga_objective<double>::manifest_d() = man;
    delete delete_man;
    
    Dijkstra* delete_dj = ga_objective<double>::dijkstra();
    //    delete delete_dj;
    ga_objective<double>::dijkstra() = d;
    
    map_data* delete_map = ga_objective<double>::map();
    delete delete_map;
    ga_objective<double>::map() = map;
    
    ga_objective<double>::num_iters() = c.ITERS();
    
    // initializing parameters lower and upper bounds
    // an initial value can be added inside the initializer list after the upper bound
    galgo::Parameter<double> alpha({1.0, 5.0});
    galgo::Parameter<double> primer({1.0, 100.0});
    galgo::Parameter<double> beta({1.0,5.0});
    galgo::Parameter<double> delta({0.0,10.0});
    galgo::Parameter<double> lambda({0.0,20.0});
    galgo::Parameter<double> phi({1.0,10.0});
    galgo::Parameter<double> rho({0.0f,1.0f});
    
    galgo::GeneticAlgorithm<double> ga(ga_objective<double>::Objective,10,50,true,alpha, primer, beta, delta, lambda, phi, rho);
    
    return ga;
}
