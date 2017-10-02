//
//  Texture.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 9/25/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "Texture.hpp"
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"
#endif

#include <iostream>
#include <string>

Texture::Texture(std::string filePath) : filePath(filePath) {
	int w, h, comp;
	unsigned char* image = stbi_load(filePath.c_str(), &w, &h, &comp, STBI_rgb_alpha);	
	if (image == NULL) {
		std::cerr << "Unable to load image: " << filePath << std::endl;
		assert(false);
	}
	GLuint retTexture;
	glGenTextures(1, &retTexture);
	glBindTexture(GL_TEXTURE_2D, retTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(image);
    textureID = retTexture;
    width = w;
    height = h;
    comp = comp;
}

GLuint Texture::getTextureID() const {
	return textureID;
}

std::string Texture::getFilePath() const {
	return filePath;
}

int Texture::getWidth() {
	return width;	
}

int Texture::getHeight() {
	return height;
}
