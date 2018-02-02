//
//  output_file.h
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-01-30.
//

#ifndef output_file_h
#define output_file_h

#include <sstream>
#include <stdio.h>

#include "Log.h"

class output_file {
    
public:
    static FILE*& stream();
    static void output(const std::string& msg);
};

inline FILE*& output_file::stream() {
    static FILE* p_stream = stderr;
    return p_stream;
}

inline void output_file::output(const std::string& msg) {
    FILE* p_stream = stream();
    if (!p_stream) {  return; }
    
    fprintf(p_stream, "%s", msg.c_str());
    fflush(p_stream);
}

typedef Log<output_file> file_log;

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL DEBUG4
#endif

#define FILE_LOG(level) \
if (level > FILELOG_MAX_LEVEL) ;\
else if (level > file_log::reporting_level() || !output_file::stream())  ;   \
else file_log().get(level)

#endif /* output_file_h */
