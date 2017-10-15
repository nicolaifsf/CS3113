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
	// Create our Game
    Game game(1080, 720, "Space Invaders");

    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
            game.InputEventUpdate(event);
		}
		// Handle User Input
        game.EventUpdate();
        
        // Any other Game Updates
        game.Update();
        
        // Redraw our Game
		game.Redraw(program);
	}

	// Cleanup our Game
	game.Cleanup();
	SDL_Quit();
	return 0;
}
