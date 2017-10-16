//
//  Game.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/15/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "Game.hpp"
#include <iostream>
#include "../MainMenu/MainMenu.h"
#include <string>
const std::string RESOURCE_FOLDER =  "NYUCodebase.app/Contents/Resources/";

Game::~Game() {
    Cleanup();
}


void Game::presetup(std::string windowName, Vector3 windowDimensions) {
    SDL_Init(SDL_INIT_VIDEO);
    // Not sure how to safely deallocate this, so not going to.
    displayWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowDimensions.getX(), windowDimensions.getY(), SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0,0,windowDimensions.getX(),windowDimensions.getY());
    
    /**Enable Blending**/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


Game::Game() : mode(STATE_MENU) {
    Vector3 window(1080.0f, 720.0f, 1.0f);
    presetup("Space Invaders", window);
    Texture* text = new Texture(RESOURCE_FOLDER + "font1.png");
    menu = new MainMenu(this, text);
    gameState = new GameState(this);
}


void Game::Update(float elapsed) {
    switch (mode) {
        case STATE_MENU:
            break;
        case STATE_GAME_LEVEL:
            gameState->Update(elapsed);
            break;
        default:
            std::cerr<<"I don't know what to do with this mode: " << mode << std::endl;
    }
}

void Game::Render(ShaderProgram& program) {
    glClear(GL_COLOR_BUFFER_BIT);
    switch (mode) {
        case STATE_MENU:
            menu->Redraw(program);
            break;
        case STATE_GAME_LEVEL:
            gameState->Redraw(program);
            break;
        default:
            std::cerr<<"I don't know what to do with this mode: " << mode << std::endl;
    }
    SDL_GL_SwapWindow(displayWindow);
}

void Game::PollInput(float elapsedTime) {
    switch (mode) {
        case STATE_MENU:
            break;
        case STATE_GAME_LEVEL:
            gameState->EventUpdate(elapsedTime);
            break;
        default:
            std::cerr << "I don't know what to do with this mode: " << mode << std::endl;
    }
}
void Game::EventInput(SDL_Event& event) {
    switch (mode) {
        case STATE_MENU:
            menu->InputEventUpdate(event);
            break;
        case STATE_GAME_LEVEL:
            gameState->InputEventUpdate(event);
            break;
        default:
            std::cerr << "I don't know what to do with this mode: " << mode << std::endl;
    }
}
void Game::setGameMode(GameMode mode) {
    this->mode = mode;
    switch (mode) {
        case STATE_MENU:
            break;
        case STATE_GAME_LEVEL:
            gameState->Setup();
            break;
        default:
            std::cerr << "I don't know what to do with this mode: " << mode << std::endl;
    }

}

Texture* Game::LoadTexture(std::string filePath) {
    std::string searchString = RESOURCE_FOLDER + filePath;
    auto iter = loadedTextures.find(searchString);
    if (iter == loadedTextures.end()) {
        Texture* t = new Texture(RESOURCE_FOLDER+filePath);
        std::pair<std::string, Texture*> p(RESOURCE_FOLDER+filePath, t);
        loadedTextures.insert(p);
        return t;
    }
    return iter->second;
}

void Game::Cleanup() {
    for(auto iter = loadedTextures.begin(); iter != loadedTextures.end(); ) {
        if (iter->second != nullptr) {
            delete iter->second;
            iter->second = nullptr;
        }
        iter = loadedTextures.erase(iter);
    }
    menu->Cleanup();
    delete menu;
    menu = nullptr;
    gameState->Cleanup();
    delete gameState;
    gameState = nullptr;
}


