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

class MainMenu {
public:
    MainMenu(Game* game, Texture* texture);
    ~MainMenu();
    void Update(float elapsedTime);
    void Redraw(ShaderProgram& program);
    void EventUpdate();
    void InputEventUpdate(SDL_Event& event);
    void Cleanup();
private:
    Texture* textTexture;
    Game* game;
};

#endif /* MainMenu_hpp */
