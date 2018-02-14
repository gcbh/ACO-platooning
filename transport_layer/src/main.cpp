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
#include <ctime>

#include "Dijkstra.hpp"
#include "ACO.hpp"
#include "cost_function.hpp"
#include "output_extractor.hpp"
#include "graph_processor.hpp"

#include "config_factory.hpp"
#include "config.hpp"
#include "map_data.hpp"
#include "manifest.hpp"
#include "graph.hpp"
#include "file_parser.hpp"

#define MAPS "../../maps/"
#define DJ_MAPS "../../maps/d_maps/"
#define DISTRIBUTION_MAPS "../../maps/distribution_centers/"
#define MANIFESTS "../../manifests/"
#define SCRIPT "../../utils/scripts/"
#define SCRIPT_NAME "process_cities_complete.py"
#define COORDS_FILENAME "cities_ids_coords_update.txt"

using namespace std;

void write_dijkstras(Dijkstra* dijkstra, string file_path, map_data map);
void write_final_output(ACO* aco, graph* g, int num_vehicles, Dijkstra *dijkstra, config conf);

int main(int argc, const char * argv[]) {

    cout << "Begin optimization" << endl;
    
    config_factory conf_fac;
    config conf = conf_fac.build();
    
    logger std_out;
    logger cost_out("../logs/cost.log", false);
    logger debug_log("../logs/debug.log", false);
    
    map_data cities_map = file_parser::parse<map_data>(map_data::data, string(MAPS) + conf.getMap());
    manifest manifest_map = file_parser::parse<manifest>(manifest::data, string(MANIFESTS) + conf.getManifest());
    
    // Creates d_maps folder if it doesn't exist
    system(("mkdir -p " + string(DJ_MAPS)).c_str());

    string dijkstra_file_path = DJ_MAPS + ("dj_" + conf.getMap());
    ifstream djfile(dijkstra_file_path);

    Dijkstra *dijkstra = new Dijkstra();
    
    // check if dijkstra file exists, if not create one
    if (djfile.fail()) {
        write_dijkstras(dijkstra, dijkstra_file_path, cities_map);
    }

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
        gp_processed_map = file_parser::parse<map_data>(map_data::dist_data ,distr_cntr_file_path);
    }

    cost_function* cost = new cost_function();
    
    ofstream output_file;
    output_file.open("../cost_per_trial.txt");
  
    for (int i = 1; i <= 5; i++) {
        int start_s=clock();

        graph *g = new graph();
        g->construct_graph(gp_processed_map);
        time_t seed = (long)time(nullptr);
        
        heuristic_selector* sel = new heuristic_selector(conf.getAlpha(), conf.getBeta(), conf.getPhi(), seed, dijkstra);
        
        ACO *aco = new ACO(g, manifest_map, conf, sel, cost, std_out, cost_out, debug_log);
        aco->init(dijkstra);
        
        double total = 0;
        
        try {
            for(int i = 1; i <= conf.ITERS(); i++) {
                int cost = aco->iteration();
                if (cost < 0) continue;
                
                if ( i > conf.ITERS() - 20) {
                    total += cost;
                }
            }
            
            double avg_cost = total/20;
            
            // generate final output
            write_final_output(aco, g, manifest_map.size(), dijkstra, conf);
            output_file << "TRIAL " << i << endl;
            output_file << "lowest cost: " << aco->get_lowest_cost() << endl;
            output_file << "dijkstra cost: " << aco->get_dijkstra_cost() << endl;
            output_file << "avg 20 iters " << avg_cost << endl;
            output_file << endl;
        
            int stop_s=clock();
            
            output_file << "Runtime " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
            output_file << endl;

            delete sel;
            delete aco;
            delete g;
            
        } catch (const exception &e) {
            cout << e.what() << endl;
        }
        
        cout << "Run " << i << " completed" << endl;
        
    }
    
    output_file.close();
    delete dijkstra;
    delete gp;
    return 0;
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

void write_final_output(ACO* aco, graph* g, int num_vehicles, Dijkstra *dijkstra, config conf) {
    string file_name = "../transport_output.json";
    output_extractor* extractor = new output_extractor(g, num_vehicles, dijkstra, file_name);
    extractor->extract_output(aco->get_dijkstra_route(), true);
    extractor->extract_output(aco->result(), false);
    extractor->pretty_print_metadata(aco->get_lowest_cost(), aco->get_dijkstra_cost(), conf);
    
    delete extractor;
}
