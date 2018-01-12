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

#include "Dijkstra.hpp"

using namespace std;
# define INF 0x3f3f3f3f
typedef pair<int, int> iPair;


Dijkstra::Dijkstra() {
    
}

Dijkstra::~Dijkstra() {
    // deallocate edge_weight
    for (int i = 0; i < num_of_nodes; i++)
        delete edge_weight[i];
    
    delete [] edge_weight;

    delete [] edg;
}

void Dijkstra::add_edge(int src, int dest, int weight) {
    edg[src].push_back(make_pair(weight, dest));
    edg[dest].push_back(make_pair(weight, src));
}

void Dijkstra::init(map_data map) {

    edg = new list<iPair> [map.node_count()];
    printf("%d", map.node_count());
    for (list<graph_edge>:: iterator itr = map.begin(); itr != map.end(); itr++) {
        add_edge(itr->src, itr->dest, itr->weight);
    }
    
    nodes = map.getNodes();
    
    for (unordered_set<int>:: iterator it = nodes.begin(); it != nodes.end(); it++) {
        shortest_route(*it);
    }

}

void Dijkstra::shortest_route (int src) {
    
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq;
    
    // vector for distances, initialized as infinite
    vector<int> wght(nodes.size(), INF);
    
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
        
        for (list< pair<int, int> >::iterator itr = edg[vertex].begin(); itr != edg[vertex].end(); itr++)
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

void Dijkstra::print_src_data(int src, vector<int> wght, int route[]) {
    
    for (int i = 0; i < nodes.size(); i++)
    {
        printf("\n %d,%d; %d; %d ", src, i, wght[i], src);
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

void Dijkstra:: populate_from_dijkstra_file(string file_name, manifest manifest_map) {
    ifstream file(file_name);
    string   line;

    // get number of unique nodes and intialize 2D array size
    getline(file, line);
    const int node_count =  stoi(line);
    edge_weight = new int*[node_count];
    for (int i = 0; i < node_count; i++)
        edge_weight[i] = new int[node_count];

    num_of_nodes = node_count;
    
    while(getline(file, line))
    {
        vector<string> columns;
        vector<string> edge;
        
        if (!line.empty()) {
            columns = split(line, ';');
            edge = split(columns[0], ',');
            int src = stoi(edge[0]);
            int dest = stoi(edge[1]);
            int distance = stod(columns[1]);
            string route = columns[2];

            // populate 2D array for an edge with weight
            edge_weight[src][dest] = distance;
            edge_weight[dest][src] = distance;

            // populate manifest routes from dijkstra's
            int key_count = manifest_map.count(src, dest);
            while (key_count > 0) {
                manifest_route.push_back(route);
                --key_count;
            }
        }
        
    }
}

int Dijkstra:: get_edge_weight(int src, int dest) {
    return edge_weight[src][dest];
}

list<string> Dijkstra:: get_manifest_routes() {
    return manifest_route;
}




