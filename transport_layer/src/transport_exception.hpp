//
//  transport_exception.hpp
//  transport_layer
//
//  Created by Geoffrey Heath on 2018-02-05.
//

#ifndef transport_exception_hpp
#define transport_exception_hpp

class transport_exception: public std::exception {
    
public:
    transport_exception(const std::string& msg);
    const char* what() const throw() {
        return _message.c_str();
    }
    
private:
    std::string _message;
};

transport_exception::transport_exception(const std::string& msg) : _message(msg) {  }

#endif /* transport_exception_hpp */
