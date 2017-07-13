//
//  TextureUtils.hpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-12.
//  Copyright © 2017 FYDP. All rights reserved.
//

#ifndef TextureUtils_hpp
#define TextureUtils_hpp

#include <stdio.h>
#include <string>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

GLuint LoadTexture(const std::string image_file_path);

#endif /* TextureUtils_hpp */
