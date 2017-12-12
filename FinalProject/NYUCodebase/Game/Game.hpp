//
//  Game.hpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/15/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include<unordered_map>
#include<string>
#include "../SheetSprite/SheetSprite.hpp"
#include "../Texture/Texture.hpp"
#include "../GameState/GameState.hpp"
#include "../ShaderProgram.h"
#include "../MainMenu/MainMenu.h"
#include "../Vector3/Vector3.hpp"

enum GameMode {STATE_MENU, STATE_GAME_LEVEL_1, STATE_GAME_LEVEL_2, STATE_GAME_LEVEL_3, STATE_QUIT, GAME_OVER, VICTORY};

class MainMenu;
class GameOver;
class GameState;
class Victory;

class Game {
public:
    Game();
    ~Game();
    // Any setup required for game
    void Setup();
    // Process updates
    void Update(float elapsed);
    // Rerender the window
    void Render(ShaderProgram& program);
    // Take any polling based input
    void PollInput(float elapsedTime);
    // Take any event based input
    void EventInput(SDL_Event& event);
    // Modify the current game mode
    void setGameMode(GameMode mode);
    // Load a texture in
    Texture* LoadTexture(std::string filePath);
    void Cleanup();
private:
    void presetup(std::string windowName, Vector3 windowDimensions);
    std::unordered_map<std::string, Texture*> loadedTextures;
    GameMode mode;
    MainMenu* menu;
    GameState* gameState;
    SDL_Window* displayWindow;
    Vector3 windowDimensions;
    GameOver* gameOver;
    Victory* victory;
};
#endif /* Game_hpp */
