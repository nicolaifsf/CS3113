#include "ShaderProgram.h"
#include "Matrix.h"
#include "GameObject/GameObject.h"

#include<iostream>

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
    #define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include "Game/Game.hpp"

int main(int argc, char *argv[])
{
    Game game(640, 360, "Pong");
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		game.EventUpdate();
        
        game.Update();
        
		game.Redraw(program);
	}

	game.Cleanup();
	SDL_Quit();
	return 0;
}
