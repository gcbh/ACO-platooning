//
//  file_parser.hpp
//  utils
//
//  Created by Geoffrey Heath on 2018-02-09.
//  Copyright © 2018 University of Waterloo. All rights reserved.
//

#ifndef file_parser_hpp
#define file_parser_hpp

#include <functional>
#include <stdio.h>
#include <sstream>
#include <fstream>

class file_parser {
    
public:
    template <typename T>
    static T parse(std::function<void (std::string, T*)> f, std::string file_name) {
        std::ifstream file(file_name);
        std::string line;
        
        T* data = new T();
        
        while(getline(file, line)) {
            
            f(line, data);
        }
        
        return *data;
    };
};

#endif /* file_parser_hpp */
