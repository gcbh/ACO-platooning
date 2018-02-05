//
//  logger.cpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-01-31.
//

#include <stdio.h>

#include "logger.h"

logger::logger(std::string file_name, bool print_headers) {
    headers = print_headers;
    if (file_name != "") {
        f = fopen(file_name.c_str(), "w");
    } else {
        f = NULL;
    }
}

void logger::log(TLogLevel level, const std::string& msg) {
    if (!f) {
        std_log::print_headers() = headers;
        STD_LOG(level) << msg;
    } else {
        file_log::print_headers() = headers;
        output_file::stream() = f;
        FILE_LOG(level) << msg;
    }
}
