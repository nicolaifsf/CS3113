//
//  GameState.hpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/15/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <stdio.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "../Entity/Entity.hpp"
#include "../Game/Game.hpp"

class Game;

class GameState {
public:
    GameState(Game* game);
    ~GameState();
    void Update(float elapsedTime);
    void Redraw(ShaderProgram& program);
    void EventUpdate(float elapsedTime);
    void InputEventUpdate(SDL_Event& event);
    void Cleanup();
    void Setup();
private:
    Game* game;
    std::unordered_map<std::string, Entity*> enemies;
    std::vector<Entity*> enemyBullets;
    std::vector<Entity*> playerBullets;
    Entity* player;
    
};
#endif /* GameState_hpp */
