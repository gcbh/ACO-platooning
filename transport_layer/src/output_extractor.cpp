
#include "output_extractor.hpp"

output_extractor::output_extractor(graph *i_g, int i_num_vehicles, Dijkstra *i_dijkstra) {
    g = i_g;
    dijkstra = i_dijkstra;
    num_vehicles = i_num_vehicles;
    transit_times = new float[num_vehicles];
    is_vehicle_platooning = new vector<bool>[num_vehicles];
    schedules = new vector<segment>*[num_vehicles];
    for (int i = 0; i < num_vehicles; i++) {
        schedules[i] = new vector<segment>();
    }
}

output_extractor::~output_extractor() {
    delete[] transit_times;
    for (int i = 0; i < num_vehicles; i++) {
        delete schedules[i];
    }
    delete is_vehicle_platooning;
    delete[] schedules;
}

void output_extractor::update_transit_time(int vehicle_id, float time) {
    transit_times[vehicle_id] = time;
}

float output_extractor::get_transit_time(int vehicle_id) {
    return transit_times[vehicle_id];
}

float output_extractor::get_max_time_for_platoon(vector<int> vehicles) {
    float max_time = -1;
    for(vector<int>::iterator l_it = vehicles.begin(); l_it != vehicles.end(); ++l_it) {
        float time = get_transit_time(*l_it);
        if (time > max_time) {
            max_time = time;
        }
    }
    
    return max_time;
}

void output_extractor::put_schedule(int vehicle_id, segment seg) {
    schedules[vehicle_id]->push_back(seg);
}

void output_extractor::make_schedule(map<iPair, vector<int> > platoons) {
    for (map<iPair, vector<int> >::iterator it = platoons.begin(); it != platoons.end(); ++it) {
        int src_id = it->first.first;
        int dest_id = it->first.second;
        t_edge *edge = (*g)[src_id]->get_edge(dest_id);
        
        // true value - not rounded
        float time_to_cross = (float) edge->get_distance() / (float) edge->get_speed();
        vector<int> vehicles = it->second;
        
        segment seg;
        seg.start_node = src_id;
        seg.end_node = dest_id;
        seg.time = time_to_cross;
        seg.max_wait = 0; // initialize to 0
        
        if (vehicles.size() > 1) {
            // platoon travel
            seg.type = "platoon_travel";
            float max_time = get_max_time_for_platoon(vehicles);
            for(vector<int>::iterator l_it = vehicles.begin(); l_it != vehicles.end(); ++l_it) {
                int vehicle_id = *l_it;
                vector<int> members = vehicles;
                
                // don't want vehicle to be part of it's own platoon list.
                members.erase(remove(members.begin(), members.end(), vehicle_id));
                
                seg.platoon_members = members;
                seg.max_wait = ceil(max_time - get_transit_time(vehicle_id));
                
                update_transit_time(vehicle_id, max_time);
                put_schedule(vehicle_id, seg);
            }
        } else {
            // solo travel
            seg.type = "solo_travel";
            int vehicle_id = vehicles.front();
            put_schedule(vehicle_id, seg);
        }
    }
}

void output_extractor::extract_output(vector<string>** schedule) {
    int max_ticks = schedule[0]->size();
    
    for (int tick = 0; tick < max_ticks; tick++) {
        map<iPair, vector<int> > platoons;
        for (int ant = 0; ant < num_vehicles; ant++) {
            vector<string>* segments = schedule[ant];
            string transit = (*segments)[tick];
            size_t pos = transit.find("->");
            
            if (pos != string::npos) {
                int start = stoi(transit.substr(0, pos));
                int end = stoi(transit.substr(pos+2));
                
                // Compute platooning information
                if (start != end) {
                    t_edge* edge = ((*g)[start])->get_edge(((*g)[end])->get_id());
                    float t_time = (float) edge->get_distance() / (float) edge->get_speed();
                    
                    // Calculate distance for ants over time
                    float transit_time = get_transit_time(ant) + t_time;
                    update_transit_time(ant, transit_time);
                    
                    iPair transit_pair = make_pair(start, end);
                    platoons[transit_pair].push_back(ant);
                }
            }
        }
        make_schedule(platoons);
    }
    pretty_print_json();
}

void output_extractor::pretty_print_json() {
    ofstream output_file;
    output_file.open("../transport_output.json");
    
    output_file << "{" << endl; // open json object
    output_file << "\t \"schedules\": [" << endl; // open schedules
    
    for (int i = 0; i < num_vehicles; i++) {
        output_file << "\t \t {" << endl; // open vehicle
        output_file << "\t \t \t \"vehicle_id\": " << i << "," << endl;
        output_file << "\t \t \t \"segments\": [" << endl; // open segments
        
        for(vector<segment>::iterator it = schedules[i]->begin(); it != schedules[i]->end(); ++it) {
        
            output_file << "\t \t \t \t {" << endl; // open segment
            
            output_file << "\t \t \t \t \t \"type\": \"" << it->type << "\"," << endl;
            output_file << "\t \t \t \t \t \"start_node\": " << it->start_node << "," << endl;
            output_file << "\t \t \t \t \t \"end_node\": " << it->end_node << "," << endl;
            output_file << "\t \t \t \t \t \"time\": " << it->time << "," << endl;
            output_file << "\t \t \t \t \t \"max_wait\": " << it->max_wait << "," << endl;
            output_file << "\t \t \t \t \t \"platoon_members\": ["; // open platoon_members
            
            vector<int> platoon = it->platoon_members;
            for (int p = 0; p < platoon.size(); p++) {
                output_file << platoon[p];
                // don't print comma if at the end of list
                if (p != platoon.size() - 1) {
                    output_file << ",";
                }
            }
            
            output_file << "]" << endl; //close platoon_members
            
            vector<segment>::iterator dupe = it;
            ++dupe;
            output_file << "\t \t \t \t }";
            if (dupe != schedules[i]->end()) {
                output_file << ",";
            }
            output_file << endl; // close segment
        }
        output_file << "\t \t \t ]" << endl; // close segments
        
        output_file << "\t \t }";
        if (i != num_vehicles - 1) {
            output_file << ",";
        }
        output_file << endl; // close vehicle
    }
    
    output_file << "\t ]" << endl; // close schedules
    output_file << "}"; // close json object
    output_file.close();
}
