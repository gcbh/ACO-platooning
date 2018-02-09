//
//  logger.cpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-01-31.
//

#include "logger.h"

const std::string logger::LOG_DIR = "logs";

logger::logger(std::string file_name, bool print_headers) {
    std::system(("mkdir -p " + logger::LOG_DIR).c_str());
    headers = print_headers;
    if (file_name != "") {
        try {
            f = fopen(file_name.c_str(), "w");
        } catch (std::exception& e) {
            STD_LOG(ERROR) << e.what();
        }
        
    } else {
        f = NULL;
    }
}

logger::~logger() {
    fclose(f);
}

void logger::log(TLogLevel level, const std::string& msg) {
    if (!f) {
        std_log::print_headers() = headers;
        STD_LOG(level) << msg;
    } else {
        file_log::print_headers() = headers;
        output_file::stream() = f;
        FILE_LOG(level) << msg << std::endl;
        fflush(f);
    }
}
