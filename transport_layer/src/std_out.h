//
//  std_out.h
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-01-30.
//

#ifndef std_out_h
#define std_out_h

#include <iostream>

#include "Log.h"

class std_out {
public:
    static void output(const std::string& msg);
};

inline void std_out::output(const std::string& msg) {
    std::cout << msg << std::endl;
}

typedef Log<std_out> std_log;

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL DEBUG4
#endif

#define STD_LOG(level) \
if (level > FILELOG_MAX_LEVEL) ;\
else std_log().get(level)

#endif /* std_out_h */
