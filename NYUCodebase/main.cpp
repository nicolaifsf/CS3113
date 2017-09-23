#include "ShaderProgram.h"
#include "Matrix.h"
#include "GameObject/GameObject.h"

// #ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// #endif

#include<iostream>


#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
    #define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

/**
TODO: Consider ways we can supply the w,h and comp values after they've been populated
Could be useful info to have, no?
**/
GLuint LoadTexture(const char *filePath) {
    int w,h,comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);
    if(image == NULL) {
        std::cerr << "Unable to load image: " << filePath << "\nMake sure the path is correct" << std::endl;
        assert(false);
    }
    
    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_image_free(image);
    return retTexture;
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Nico's Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif

    /**SETUP**/
    glViewport(0,0,640,360);

    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

    GLuint marioTexture = LoadTexture(RESOURCE_FOLDER"mario.png");
    GLuint emojiiTexture = LoadTexture(RESOURCE_FOLDER"smiley.png");
    GLuint squidwardTexture = LoadTexture(RESOURCE_FOLDER"squidward.png"); 

    GameObject mario(marioTexture);
    GameObject emojii(emojiiTexture);
    GameObject squidward(squidwardTexture);
    emojii.moveRight(3.0f);
    emojii.moveDown(0.5f);
    squidward.moveLeft(3.0f);
    squidward.moveUp(0.5f);

    /**Enable Blending**/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
        
		glClear(GL_COLOR_BUFFER_BIT);

        mario.draw(program);
        emojii.draw(program);
        squidward.draw(program);
        
		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}
