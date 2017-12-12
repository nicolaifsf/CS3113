//
//  Victory.cc
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/15/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "Victory.h"
#include "../Utilities/TextUtility.hpp"
#include "../Game/Game.hpp"
#include<iostream>

Victory::Victory(Game* game, Texture* texture) : game(game), textTexture(texture){}

Victory::~Victory(){
    Cleanup();
}

void Victory::Update(float elapsedTime) {
    
}

void Victory::Redraw(ShaderProgram& program) {
    Textbox(program, textTexture, "You Win!", -2.2f, 1.8f);
    Textbox(program, textTexture, "Press SPACE to go to menu", -2.2f, 1.2f);
}

void Victory::EventUpdate() {
    
}

void Victory::InputEventUpdate(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
            game->setGameMode(STATE_MENU);
        } else if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            game->setGameMode(STATE_MENU); 
        }
    }
}

void Victory::Cleanup() {
}
