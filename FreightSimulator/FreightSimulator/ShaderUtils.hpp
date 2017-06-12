//
//  ShaderUtils.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-06-12.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef ShaderUtils_hpp
#define ShaderUtils_hpp

#include <stdio.h>
#include <string>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

GLuint LoadShaders(const std::string vertex_file_path, const std::string fragment_file_path);

#endif /* ShaderUtils_hpp */
