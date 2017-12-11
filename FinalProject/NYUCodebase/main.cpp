#include "ShaderProgram.h"
#include "Matrix.h"
#include<iostream>

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
    #define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#include "Game/Game.hpp"

#define FIXED_TIMESTEP 0.0166666f

int main(int argc, char *argv[])
{
	// Create our Game
    Game* game = new Game();
    float lastFrameTicks = (float) SDL_GetTicks()/1000.0f;
    float elapsedTime = 0.0f;
    float accumulator = 0.0f;
    ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");

	SDL_Event event;
	bool done = false;
	while (!done) {
        float ticks = (float)SDL_GetTicks()/1000.0f;
        elapsedTime = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        elapsedTime += accumulator;
        if(elapsedTime < FIXED_TIMESTEP) {
            accumulator = elapsedTime;
            continue;
        }
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
            game->EventInput(event);
        }
        // Handle User Input
        game->PollInput(FIXED_TIMESTEP);
        while(elapsedTime >=FIXED_TIMESTEP) {


            // Any other Game Updates
            game->Update(FIXED_TIMESTEP);
            

            elapsedTime -= FIXED_TIMESTEP;
        }
        accumulator = elapsedTime;

        // Redraw our Game
        game->Render(program);
	}

	// Cleanup our Game
    game->Cleanup();
	SDL_Quit();
	return 0;
}
