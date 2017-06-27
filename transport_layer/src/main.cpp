//
//  main.cpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2017-06-06.
//
//

#include <stdio.h>
#include <list>
#include <iostream>
#include <map>
#include <fstream>
#include <sstream>


#include "../../models/graph.hpp"
#include "Dijkstra.hpp"
#include "ACO_new.hpp"

using namespace std;

int main() {
    cout << "hello world" << endl;

    string file_name = "cities_p.txt";
    
    // open graph file, read and pass data to Dijkstra to calculate shortest path
    ifstream file("../../" + file_name);
    string   line;

    int speed = 100; //fixed speed on every edge for now.
    
    Dijkstra *dijkstra = new Dijkstra();
    list<graph_data> pre_opt_graph;

    while(getline(file, line))
    {
        stringstream   linestream(line);
        string         data;
        int            src;
        int            dest;
        string         src_name;
        string         dest_name;
        double         distance;

        
        linestream >> src >> src_name >> dest >> dest_name >> distance;
        
        struct graph_data edge;
        edge.src = src;
        edge.dest = dest;
        edge.weight = distance;
        
        pre_opt_graph.push_back(edge);
    }
    
    string out_file_name = "dj_" + file_name;
    ifstream djfile("../../" + out_file_name);

    // check if dijkstra file exists, if not create one
    if (djfile.fail()) {

        // create file to output from dijkstra algorithm
        ofstream dijkstra_file;
        string dijkstra_file_path = "../../" + out_file_name;
        dijkstra_file.open(dijkstra_file_path, ios_base::out);
        

        // freopen to be able to write output to file directly
        freopen(dijkstra_file_path.c_str(), "w", stdout);
        dijkstra->init(pre_opt_graph);
        fclose(stdout);

    } 

    graph *g = new graph();
    g->construct_graph(pre_opt_graph); 
    
    ACO_new *ACO = new ACO_new(g, 10);
    return 0;
}
