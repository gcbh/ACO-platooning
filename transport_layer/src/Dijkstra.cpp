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


Dijkstra::Dijkstra () {
    edg = new list<iPair> [200];
}

Dijkstra::~Dijkstra() {
    
}

void Dijkstra:: add_edge(int src, int dest, double weight) {
    edg[src].push_back(make_pair(weight, dest));
    edg[dest].push_back(make_pair(weight, src));
}


void Dijkstra:: init(list<graph_data> edge_list) {

    for (list<graph_data>:: iterator itr = edge_list.begin(); itr != edge_list.end(); itr++) {
        nodes.insert(itr->src);
        nodes.insert(itr->dest);
        add_edge(itr->src, itr->dest, itr->weight);
    }
    
    for (set<int>:: iterator it = nodes.begin(); it != nodes.end(); it++) {
        shortest_route(*it);
    }

}


void Dijkstra:: shortest_route (int src) {
    
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

void Dijkstra:: print_src_data(int src, vector<double> wght, int route[]) {
    
    for (int i = 0; i < nodes.size(); i++)
    {
        printf("\n(%d, %d); %f; %d ", src, i, wght[i], src);
        print_path(route, i);
    }
}

void Dijkstra:: print_path(int route[], int j)
{
    if (route[j] == -1)
        return;
    
    print_path(route, route[j]);

    printf("%d ", j);
}


