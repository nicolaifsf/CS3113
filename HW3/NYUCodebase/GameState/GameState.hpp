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

// GameState maintains the state of our game
class GameState {
public:
    GameState(Game* game);
    ~GameState();
    // Update - process any movement, check for collisions, check for win or loss
    void Update(float elapsedTime);
    // Redraw
    void Redraw(ShaderProgram& program);
    // EventUpdate - Process any polling events
    void EventUpdate(float elapsedTime);
    // InputEventUpdate - Process any input events
    void InputEventUpdate(SDL_Event& event);
    // Cleanup
    void Cleanup();
    // Setup
    void Setup();
private:
    Game* game;
    std::unordered_map<std::string, Entity*> enemies;
    std::vector<Entity*> enemyBullets;
    std::vector<Entity*> playerBullets;
    Entity* player;
    
};
#endif /* GameState_hpp */
