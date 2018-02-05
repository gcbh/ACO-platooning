//
//  logger.h
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-01-30.
//

#ifndef logger_h
#define logger_h

#include <cstdio>
#include <sstream>

#include "std_out.h"
#include "output_file.h"

class logger {
public:
    logger(std::string file_name = "", bool print_headers = true);
    ~logger();
    void log(TLogLevel level, const std::string& msg);
private:
    FILE* f;
    bool headers;
};

#endif /* logger_h */
