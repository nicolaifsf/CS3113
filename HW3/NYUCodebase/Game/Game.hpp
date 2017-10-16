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

enum GameMode {STATE_MENU, STATE_GAME_LEVEL};

class MainMenu;
class GameState;

class Game {
public:
    Game();
    ~Game();
    void Setup();
    void Update(float elapsed);
    void Render(ShaderProgram& program);
    void PollInput(float elapsedTime);
    void EventInput(SDL_Event& event);
    void setGameMode(GameMode mode);
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
};
#endif /* Game_hpp */
