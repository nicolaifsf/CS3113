//
//  MainMenu.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/15/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "MainMenu.h"
#include "../Utilities/TextUtility.hpp"
#include "../Game/Game.hpp"
#include<iostream>

MainMenu::MainMenu(Game* game, Texture* texture) : game(game), textTexture(texture){}

MainMenu::~MainMenu(){
    Cleanup();
}

void MainMenu::Update(float elapsedTime) {
    
}

void MainMenu::Redraw(ShaderProgram& program) {
    Textbox(program, textTexture, "Jumper", -2.2f, 1.8f);
    Textbox(program, textTexture, "Press 1 to start level 1", -2.2f, 1.2f);
    Textbox(program, textTexture, "Press 2 to start level 2", -2.2f, 0.6f);
    Textbox(program, textTexture, "Press 3 to start level 3", -2.2f, 0.0f);

    Textbox(program, textTexture, "Press ESC to quit", -2.2f, -1.0f);


}

void MainMenu::EventUpdate() {
    
}

void MainMenu::InputEventUpdate(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_1) {
            game->setGameMode(STATE_GAME_LEVEL_1);
        } else if (event.key.keysym.scancode == SDL_SCANCODE_2) {
            game->setGameMode(STATE_GAME_LEVEL_2); 
        } else if (event.key.keysym.scancode == SDL_SCANCODE_3) {
            game->setGameMode(STATE_GAME_LEVEL_3);
        } else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            game->setGameMode(STATE_QUIT);
        }
    }
}

void MainMenu::Cleanup() {

}
