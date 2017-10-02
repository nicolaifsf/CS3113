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


// #ifdef _WINDOWS
// 	#define RESOURCE_FOLDER ""
// #else
//     #define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
// #endif

#include <string>

class Texture {
public:
    Texture(std::string filePath);
	GLuint getTextureID() const;
	std::string getFilePath() const;
    int getWidth();
    int getHeight();
private:
	GLuint textureID;
	std::string filePath;
	int width;
	int height;
	int comp;
};

#endif /* Texture_hpp */
