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
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <stdio.h>
#include<unordered_map>
#include "../Texture/Texture.hpp"
#include "../GameObject/GameObject.h"
#include <string>
#include <vector>

// Game is our representation of the running Game
class Game {
public:
    // Constructor - specify window width, height, and name
	Game(int width, int height, std::string windowName);
    // Destructor - Deallocates Game Objects and Textures
    ~Game();
    // LoadTexture - loads a texture based on filePath.
    Texture* LoadTexture(std::string filePath);
    // AddObject - adds an allocated object to the game's unordered_map containing all gameobjects
    bool AddObject(GameObject* object);
    // NewObject - Creates a new object with the given name and texture filepath
    GameObject* NewObject(std::string name, const char* filePath);
    // Update - Any non user event based updates go in here
    void Update();
    // Redraw - Loops through all our game objects and redraws them to the screen
    void Redraw(ShaderProgram& program);
    // EventUpdate - User input events go in here
    void EventUpdate();
    // GetObject - Retrieve a GameObject* based on its name (id)
    GameObject* GetObject(std::string name);
    // Cleanup - Deallocates any GameObjects and Textures that have been added
    void Cleanup();
private:
    // updateElapsedtime - Updates the amount of elapsed time that has passed in our game
    void updateElapsedTime();
    // PreSetup - The typical OpenGL functions required for setup
    void PreSetup(std::string windowName);
    // Setup - This function we create all of our GameObjects and Textures as well as 
    // anything else we plan to use throughout the course of the game
    void Setup();
    // loadedTextures - an unordered_map mapping filePaths to Texture Object pointers
    std::unordered_map<std::string, Texture*> loadedTextures;
    // width -The width of our game screen
    int width;
    // height -The height of our game screen
    int height;
    // displayWindow - the SDL game window.
    SDL_Window* displayWindow;
    // loadedObjects - map of all GameObjects. ID/name of object maps to GameObject*
    std::unordered_map<std::string, GameObject*> loadedObjects;
    // lastFrameTicks - The ticks from last frame
    float lastFrameTicks;
    // elapsedTime - The amount of time elapsed
    float elapsedTime;
    // timeUpdated - Wether or not the time has been updated for this frame.
    bool timeUpdated;
};

#endif /* Game_hpp */
