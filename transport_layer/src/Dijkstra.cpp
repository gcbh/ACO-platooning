  //
//  Dijkstra.cpp
//
//  Created by Aditi Lath on 2017-06-09.
//  Copyright © 2017 Aditi Lath. All rights reserved.
//
//
//  Dijkstra.cpp
//  transport_layer
//
//  Created by Aditi Lath on 2017-06-08.
//
//  Add license http://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/



#include <stdio.h>
#include <iostream>
#include <queue>
#include <list>
#include <set>
#include <fstream>
#include <sstream>
#include "Dijkstra.hpp"

using namespace std;
# define INF 0x3f3f3f3f
typedef pair<double, int> iPair;


Dijkstra::Dijkstra() {
    // edg = new list<iPair> [200];
}

Dijkstra::~Dijkstra() {
    // delete edg
    delete [] edg;

    // deallocate edge_weight
    for (int i = 0; i < num_of_nodes; i++)
        delete [] edge_weight[i];
    
    delete [] edge_weight;
}

void Dijkstra::add_edge(int src, int dest, double weight) {
    edg[src].push_back(make_pair(weight, dest));
    edg[dest].push_back(make_pair(weight, src));
}

void Dijkstra::init(list<graph_data> edge_list, int node_count) {

    edg = new list<iPair> [node_count];
    for (list<graph_data>:: iterator itr = edge_list.begin(); itr != edge_list.end(); itr++) {
        nodes.insert(itr->src);
        nodes.insert(itr->dest);
        add_edge(itr->src, itr->dest, itr->weight);
    }

    num_of_nodes = nodes.size();
    printf("%d ", num_of_nodes);
    for (set<int>:: iterator it = nodes.begin(); it != nodes.end(); it++) {
        shortest_route(*it);
    }

}

void Dijkstra::shortest_route (int src) {
    
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq;
    
    // vector for distances, initialized as infinite
    vector<double> wght(nodes.size(), INF);
    
    // Array to store shortest path route
    int route[nodes.size()];
    route[src] = -1;
    
    // Insert source itself in priority queue with distance as 0
    pq.push(make_pair(0, src));
    wght[src] = 0;
    
    
    
    while (!pq.empty())
    {
        // get the adjacent vertex for the element
        int vertex = pq.top().second;
        pq.pop();
        
        for (list< pair<double, int> >::iterator itr = edg[vertex].begin(); itr != edg[vertex].end(); itr++)
        {
            int adj = itr->second;
            int weight = itr->first;
            
            if (wght[adj] > wght[vertex] + weight)
            {
                // Add current node to route for given source
                route[adj] = vertex;

                // distance update for adjacent vertex
                wght[adj] = wght[vertex] + weight;
                pq.push(make_pair(wght[adj], adj));
            }
        }
    }
    
    print_src_data(src, wght, route);
    
}

void Dijkstra::print_src_data(int src, vector<double> wght, int route[]) {
    
    for (int i = 0; i < nodes.size(); i++)
    {
        printf("\n %d,%d; %f; %d ", src, i, wght[i], src);
        print_path(route, i);
    }
}

void Dijkstra::print_path(int route[], int j)
{
    if (route[j] == -1)
        return;
    
    print_path(route, route[j]);

    printf("%d ", j);
}

void Dijkstra:: populate_weight(string file_name) {
    ifstream file("../../" + file_name);
    string   line;

    getline(file, line);
    const int node_count =  stoi(line);
    edge_weight = new double*[node_count];
    for (int i = 0; i < node_count; i++)
        edge_weight[i] = new double[node_count];


    while(getline(file, line))
    {
        vector<string> columns;
        vector<string> edge;
        

        if (!line.empty()) {
            columns = split(line, ';');
            edge = split(columns[0], ',');
            int src = stoi(edge[0]);
            int dest = stoi(edge[1]);
            double distance = stod(columns[1]);

            edge_weight[src][dest] = distance;
            edge_weight[dest][src] = distance;
            // vec.push_back(dest);
            // if (src < dest) {
            //     set_vector(src,dest);
                
            //     edge_weight.at(src).at(dest) = 10;
            // } else {
            //     set_vector(dest,src);
            //     // edge_weight.at(dest).push_back(src);
            //     edge_weight.at(dest).at(src) = 11;
            // }
            
        }
        
    }
}

//void Dijkstra:: set_vector(int position, int next_pos) {
    // try{
    //     // check if index exists in vector else throw error
    //     bool exists = edge_weight.at(position).empty();
        
    //     try {
    //         // check if next_post exists in vector, if not throws exception
    //         int value = edge_weight.at(position).at(next_pos);
    //     } catch (const out_of_range& oor) {
    //         edge_weight.at(position).push_back(next_pos);
    //     }
            
    // } 
    // catch (const out_of_range& oor) {
    //     // index doesn't exist, add it to 2D vector
    //     vector<int> vec; 
    //     edge_weight.push_back(vec);
    //     edge_weight.at(position).push_back(next_pos);
    // }
//}

vector<string> Dijkstra:: split(const string &s, char delim)
{
    stringstream ss(s);
    string item;
    vector<string> elems;
    
    while (getline(ss, item, delim)) {
            elems.push_back(item);
    }
    return elems;
}

double Dijkstra:: get_edge_weight(int src, int dest) {
    return edge_weight[src][dest];
}


