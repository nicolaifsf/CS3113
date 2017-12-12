//
//  GameOver.cc
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/15/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "GameOver.h"
#include "../Utilities/TextUtility.hpp"
#include "../Game/Game.hpp"
#include<iostream>

GameOver::GameOver(Game* game, Texture* texture) : game(game), textTexture(texture){}

GameOver::~GameOver(){
    Cleanup();
}

void GameOver::Update(float elapsedTime) {
    
}

void GameOver::Redraw(ShaderProgram& program) {
    Textbox(program, textTexture, "Game Over", -2.2f, 1.8f);
    Textbox(program, textTexture, "Press SPACE to go to menu", -2.2f, 1.2f);
}

void GameOver::EventUpdate() {
    
}

void GameOver::InputEventUpdate(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
            game->setGameMode(STATE_MENU);
        } else if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            game->setGameMode(STATE_MENU); 
        }
    }
}

void GameOver::Cleanup() {
}
