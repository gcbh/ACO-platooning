
#ifndef output_extractor_hpp
#define output_extractor_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <list>
#include <vector>
#include <cmath>
#include "Dijkstra.hpp"
#include "../../models/graph.hpp"
#include "../../models/t_node.hpp"
#include "../../models/config.hpp"

using namespace std;
typedef pair<int, int> iPair;

struct segment {
    string type;
    int start_node;
    int end_node;
    float time;
    int max_wait;
    vector<int> platoon_members;
};

class output_extractor {
public:
    output_extractor(graph *i_g, int i_num_vehicles, Dijkstra *i_dijkstra);
    virtual ~output_extractor ();
    void    extract_output(vector<string>** schedule, string output_file_name, bool is_dijkstra);
    void    pretty_print_metadata(string output_file_name, double cost, double dijkstra_cost, config conf);
private:
    graph*                              g;
    Dijkstra*                           dijkstra;
    vector<segment>**                   schedules;
    float*                              transit_times;
    int                                 num_vehicles;
    int                                 total_cost_change;
    vector<bool>*                       is_vehicle_platooning;
    void    make_schedule(map<iPair, vector<int> > platoons);
    float   get_max_time_for_platoon(vector<int> vehicles);
    void    put_schedule(int vehicle_id, segment seg);
    void    update_transit_time(int vehicle_id, float time);
    float   get_transit_time(int vehicle_id);
    void    fetch_dijkstra_for_non_platooning();
    int     fetch_vehicle_old_cost(int vehicle_num);
    void    vehicle_cost_change(int vehicle_num, string route);
    void    pretty_print_json(string output_file_name, bool is_dijkstra);
    void    reset();
};

#endif /* output_extractor_hpp */
