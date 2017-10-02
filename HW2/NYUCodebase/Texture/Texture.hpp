//
//  Texture.hpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 9/25/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>


#include <string>

// Representation of a Texture we've loaded into our game
class Texture {
public:
	// Create a new texture based on filePath to resource
    Texture(std::string filePath);
    // Get the ID reference of the texture
	GLuint getTextureID() const;
	// Get the filePath of the texture
	std::string getFilePath() const;
	// Get the Width of the texture
    int getWidth();
    // Get the Height of the texture
    int getHeight();
private:
	GLuint textureID;
	std::string filePath;
	int width;
	int height;
	int comp;
};

#endif /* Texture_hpp */
