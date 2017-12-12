//
//  Victory.h
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/15/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef VICTORY_H 
#define VICTORY_H 
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdio.h>
#include "../ShaderProgram.h"
#include "../Texture/Texture.hpp"
#include "../Game/Game.hpp"

class Game;

class Victory{
public:
    Victory(Game* game, Texture* texture);
    ~Victory();
    // Process any updates to the Victory
    void Update(float elapsedTime);
    // Redraw the Victory screen
    void Redraw(ShaderProgram& program);
    // Process any polling events
    void EventUpdate();
    // Process any Input Events
    void InputEventUpdate(SDL_Event& event);
    // Cleanup
    void Cleanup();
private:
    // The texture used for menus
    Texture* textTexture;
    // Pointer to the parent game object
    Game* game;
};

#endif /* MainMenu_hpp */
