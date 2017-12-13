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
#include "../GameOver/GameOver.h"
#include "../Victory/Victory.h"
#include <string>
#include <SDL_Mixer.h>

const std::string RESOURCE_FOLDER =  "NYUCodebase.app/Contents/Resources/";

void quit() {
    SDL_Event quit_event;
    quit_event.type=SDL_QUIT;
    SDL_PushEvent(&quit_event);
}

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


Game::Game() : mode(STATE_MENU),gameState(nullptr) {
    Vector3 window(1080.0f, 720.0f, 1.0f);
    presetup("Jumper", window);
    Texture* fontSheet = LoadTexture("font1.png");
    menu = new MainMenu(this, fontSheet);
    gameOver = new GameOver(this, fontSheet);
    victory = new Victory(this, fontSheet);
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    gameOverMusic = Mix_LoadMUS("sad.mp3");
    victoryMusic = Mix_LoadMUS("yay.mp3");
}


void Game::Update(float elapsed) {
    switch (mode) {
        case STATE_MENU:
            break;
        case STATE_GAME_LEVEL_1:
            gameState->Update(elapsed);
            break;
        case STATE_GAME_LEVEL_2:
            gameState->Update(elapsed);
            break;
        case STATE_GAME_LEVEL_3:
            gameState->Update(elapsed);
            break;
        case STATE_QUIT:
            break;
        case GAME_OVER:
            break;
        case VICTORY:
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
        case STATE_GAME_LEVEL_1:
            gameState->Redraw(program);
            break;
        case STATE_GAME_LEVEL_2:
            gameState->Redraw(program);
            break;
        case STATE_GAME_LEVEL_3:
            gameState->Redraw(program);
            break;
        case STATE_QUIT:
            break;
        case GAME_OVER:
            gameOver->Redraw(program);
            break;
        case VICTORY:
            victory->Redraw(program);
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
        case STATE_GAME_LEVEL_1:
            gameState->EventUpdate(elapsedTime);
            break;
        case STATE_GAME_LEVEL_2:
            gameState->EventUpdate(elapsedTime);
            break;
        case STATE_GAME_LEVEL_3:
            gameState->EventUpdate(elapsedTime);
            break;
        case STATE_QUIT:
            // quit();
            break;
        case GAME_OVER:
            break;
        case VICTORY:
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
        case STATE_GAME_LEVEL_1:
            gameState->InputEventUpdate(event);
            break;
        case STATE_GAME_LEVEL_2:
            gameState->InputEventUpdate(event);
            break;
        case STATE_GAME_LEVEL_3:
            gameState->InputEventUpdate(event);
            break;
        case STATE_QUIT:
            break;
        case GAME_OVER:
            gameOver->InputEventUpdate(event);
            break;
        case VICTORY:
            victory->InputEventUpdate(event);
            break;
        default:
            std::cerr << "I don't know what to do with this mode: " << mode << std::endl;
    }
}
void Game::setGameMode(GameMode mode) {
    this->mode = mode;
    switch (mode) {
        case STATE_MENU:
            if(gameState != nullptr) {
                gameState->Cleanup();
                delete gameState;
                gameState = nullptr;
            }
            break;
        case STATE_GAME_LEVEL_1:
            gameState = new GameState(this);
            gameState->Setup(1);
            break;
        case STATE_GAME_LEVEL_2:
            gameState = new GameState(this);
            gameState->Setup(2);
            break;
        case STATE_GAME_LEVEL_3:
            gameState = new GameState(this);
            gameState->Setup(3);
            break;
        case GAME_OVER:
            if(gameState != nullptr) {
                gameState->Cleanup();
                delete gameState;
                gameState = nullptr;
            }
            Mix_PlayMusic(gameOverMusic, 1);
            break;
        case STATE_QUIT:
            if(gameState != nullptr) {
                gameState->Cleanup();
                delete gameState;
                gameState = nullptr;
            }
            quit();
            break;
        case VICTORY:
            if(gameState != nullptr) {
                gameState->Cleanup();
                delete gameState;
                gameState = nullptr;
            }
            Mix_PlayMusic(victoryMusic, 1);
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
    if(gameState != nullptr) {
        gameState->Cleanup();
        delete gameState;
        gameState = nullptr;
    }
    if(gameOver != nullptr) {
        gameOver->Cleanup();
        delete gameOver;
        gameOver = nullptr;
    }
    if(menu != nullptr) {
        delete menu; 
        menu=nullptr;
    }
    if(victory != nullptr) {
        victory->Cleanup();
        delete victory;
        victory = nullptr;
    }
    if(gameOverMusic != nullptr) {
        Mix_FreeMusic(gameOverMusic);
        gameOverMusic = nullptr;
    }
    if(victoryMusic != nullptr) {
        Mix_FreeMusic(victoryMusic);
        victoryMusic = nullptr;
    }
    Mix_CloseAudio();
}


