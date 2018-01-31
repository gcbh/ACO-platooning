//
//  Log.h
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-01-30.
//

#ifndef Log_h
#define Log_h

#include <ctime>

#include "log_level.h"

template <typename T>
class Log {
public:
    Log();
    virtual ~Log();
    std::ostringstream& get(TLogLevel level = INFO);
    
    static bool& print_headers();
    static TLogLevel& reporting_level();
    static std::string to_string(TLogLevel level);
    static TLogLevel from_string(const std::string& level);
    
protected:
    std::ostringstream os;
    
private:
    Log(const Log&);
    Log& operator =(const Log&);
    
    TLogLevel message_level;
    
};

template <typename T>
Log<T>::Log() {  }

template <typename T>
inline bool& Log<T>::print_headers() {
    static bool temp = true;
    return temp;
}

template <typename T>
std::ostringstream& Log<T>::get(TLogLevel level) {
    if (print_headers()) {
        //    time_t t = time(0);
        //    struct tm * now = localtime( & t );
        //    os << "- " << t;
        os << " " << to_string(level) << ": ";
//        os << std::string(level > DEBUG ? 0 : level - DEBUG, '\t');
    }
    message_level = level;
    return os;
}

template <typename T>
Log<T>::~Log() {
    os << std::endl;
    T::output(os.str());
}

template <typename T>
TLogLevel& Log<T>::reporting_level() {
    static TLogLevel message_level = DEBUG4;
    return message_level;
}

template <typename T>
std::string Log<T>::to_string(TLogLevel level) {
    static const char* const buffer[] = {"ERROR", "WARNING", "INFO", "DEBUG", "DEBUG1", "DEBUG2", "DEBUG3", "DEBUG4"};
    return buffer[level];
}

template <typename T>
TLogLevel Log<T>::from_string(const std::string& level) {
    if (level == "ERROR") { return ERROR; }
    if (level == "WARNING") { return WARNING; }
    if (level == "INFO") { return INFO; }
    if (level == "DEBUG") { return DEBUG; }
    if (level == "DEBUG1") { return DEBUG1; }
    if (level == "DEBUG2") { return DEBUG2; }
    if (level == "DEBUG3") { return DEBUG3; }
    if (level == "DEBUG4") { return DEBUG4; }
    Log<T>().get(WARNING) << "Unknown logging level";
    return INFO;
}
#endif /* Log_h
*/
