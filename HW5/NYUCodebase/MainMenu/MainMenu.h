//
//  MainMenu.hpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/15/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef MainMenu_hpp
#define MainMenu_hpp
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

// NOT USED
// MainMenu displayed at startup and at end of game
class MainMenu {
public:
    MainMenu(Game* game, Texture* texture);
    ~MainMenu();
    // Process any updates to the MainMenu
    void Update(float elapsedTime);
    // Redraw the main menu
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
