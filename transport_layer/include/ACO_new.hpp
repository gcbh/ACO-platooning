//
//  ACO_new.hpp
//  transport_layer
//
//  Created by Priya Bibra on 2017-06-08.
//
//

#ifndef ACO_new_hpp
#define ACO_new_hpp

#include "../aco/include/Randoms.h"
#include "../../models/t_node.hpp"
#include <map>

class ACO_new {
public:
    std::map<int, t_node> *graph;
    ACO_new ();
    virtual ~ACO_new ();
private:
    
};

#endif /* ACO_new_hpp */
