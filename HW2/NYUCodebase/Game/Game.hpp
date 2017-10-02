//
//  Game.hpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 9/25/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
//TODO: Probably don't need all these imports...
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <stdio.h>
#include<unordered_map>
// TODO: Fix these includes
#include "../Texture/Texture.hpp"
#include "../GameObject/GameObject.h"
#include <string>
#include <vector>

class Game {
public:
	Game(int width, int height, std::string windowName);
    Texture* LoadTexture(std::string filePath);
    bool addObject(GameObject* object);
    // Creates a new object and saves it to the game
    GameObject* NewObject(std::string name, const char* filePath);
    void Update();
    void Redraw(ShaderProgram& program);
    void EventUpdate();
    GameObject* GetObject(std::string name);
private:
    void updateElapsedTime();
    void PreSetup(std::string windowName);
    void Setup();
    std::unordered_map<std::string, Texture*> loadedTextures;
    int width;
    int height;
    SDL_Window* displayWindow;
    std::unordered_map<std::string, GameObject*> loadedObjects;
    float lastFrameTicks;
    float elapsedTime;
    bool timeUpdated;
};

#endif /* Game_hpp */
