//
//  TextureUtils.cpp
//  FreightSimulator
//
//  Created by Sean Amadio on 2017-07-12.
//  Copyright © 2017 FYDP. All rights reserved.
//

#include <iostream>
#include "TextureUtils.hpp"
#include "lodepng.h"

GLuint LoadTexture(const std::string image_file_path) {

    std::cout << "Loading texture: " << image_file_path << std::endl;

    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, image_file_path);

    if(error != 0) {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
    }

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    // When MAGnifying the image (no bigger mipmap available), use LINEAR filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // When MINifying the image, use a LINEAR blend of two mipmaps, each filtered LINEARLY too
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // Generate mipmaps, by the way.
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return textureID;
}
