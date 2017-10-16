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
    Textbox(program, textTexture, "Space Invaders", -1.4f, 1.8f);
    Textbox(program, textTexture, "Press ENTER to start", -1.8f, 1.6f);
}
void MainMenu::EventUpdate() {
    
}
void MainMenu::InputEventUpdate(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
            game->setGameMode(STATE_GAME_LEVEL);
        }
    }
}
void MainMenu::Cleanup() {
    delete textTexture;
    textTexture= nullptr;
}
