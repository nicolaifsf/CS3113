//
//  Game.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 9/25/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "Game.hpp"

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

#include<SDL.h>
#include<SDL_opengl.h>
#include<SDL_image.h>

#include "../Texture/Texture.hpp"
#include "../GameObject/GameObject.h"
#include "../Menu/Menu.hpp"
#include"../Utilities/TextUtility.hpp"

#include <string>
#include <iostream>
#include <cassert>

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
    #define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

const float PLAYER_MOVEMENT_SPEED = 5.0f;
const int NUM_ENEMIES = 25;
const int NUM_PLAYER_BULLETS = 5;
const int NUM_ENEMY_BULLETS = 50;
const int CHANCE_BULLET = 20;

Game::Game(int width, int height, std::string windowName) : width(width), height(height) {
	PreSetup(windowName);
    /**General Setup**/
    lastFrameTicks = 0.0f;
    timeUpdated = false;
    displayMenu = true;
    menu = new Menu(LoadTexture(RESOURCE_FOLDER"font1.png"));
}

// Loads a texture based on filePath. Checks if the texture has already been loaded
// If it hasn't been loaded already, then load the file.
// If it has been loaded already, just return the Texture*
Texture* Game::LoadTexture(std::string filePath) {
	auto iter = loadedTextures.find(filePath);
	if (iter == loadedTextures.end()) {
        Texture* t = new Texture(filePath);
		std::pair<std::string, Texture*> p(filePath, t);
		loadedTextures.insert(p);
		return t;
	}
	return iter->second;
}

// Create a new GameObject with the specified Name and filePath
// Fails to add the object if an object with the same name has already been added
GameObject* Game::NewObject(std::string name, const char* filePath) {
	Texture* texture = LoadTexture(filePath);
    GameObject* newObject = new GameObject(name, texture);
    if(!AddObject(newObject)) {
        std::cerr << "Failed to add object!" << std::endl;
        delete newObject;
        return nullptr;
    }
    return newObject;
}

// The typical SDL and OpenGL setup that occurs as well as setting up some of our Game's specific options
void Game::PreSetup(std::string windowName) {
	SDL_Init(SDL_INIT_VIDEO);
    // Not sure how to safely deallocate this, so not going to.
	displayWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();	
	#endif
	glViewport(0,0,width,height);

	/**Enable Blending**/
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

// Setup any game objects that we are utilizing
void Game::Setup() {
    srand(time(NULL));
    // Initialize the player
    GameObject* player = NewObject("player", RESOURCE_FOLDER"RedPaddle.png");
    player->SetPosition(0.0f, -1.9f);
    player->Scale(.01f, .001f, 0.0f);
    
    // Initialize the enemies
    float spaceBetween = .6f;
    float initialXPos = -3.0f;
    float lastXPos = initialXPos;
    float lastYPos = 1.5f;
    for(int x = 1; x < NUM_ENEMIES; ++x) {
        GameObject* enemy = NewObject("enemy" + std::to_string(x), RESOURCE_FOLDER"ball.png");
        enemy->Scale(.01f, .01f, .01f);
        enemy->SetPosition(lastXPos, lastYPos);
        lastXPos += spaceBetween;
        if (x%8 == 0) {
          lastYPos += -spaceBetween;
          lastXPos = initialXPos;
        }
    }
}

// helper function I added to flash the display window with the scoring side's colors
void flash(SDL_Window* displayWindow, float r, float g, float b) {
    for (int i = 0; i < 5; ++i) {
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(displayWindow);
        
        glClearColor(r,g,b, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(displayWindow);
        
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(displayWindow);
    }
}

bool rightHitWall(GameObject* obj) {
    float rightOfObject = obj->getXPosition() + obj->getWidth()/2.0f;
    return rightOfObject > 3.55f;
}

bool leftHitWall(GameObject* obj) {
    float leftOfObject = obj->getXPosition() - obj->getWidth()/2.0f;
    return leftOfObject < -3.55f;
}

bool didHitWall(GameObject* obj) {
    return leftHitWall(obj) || rightHitWall(obj);
}

bool enemyShouldFire(size_t numEnemyBullets) {
    return NUM_ENEMY_BULLETS > numEnemyBullets && rand()%100 < CHANCE_BULLET;
}

// Any updates that are not based on user input.
// Also we perform the collision detection here
void Game::Update() {
    if (displayMenu) {
        return;
    }
    updateElapsedTime();

    // Move all enemies in a direction
    float m_x_pos = elapsedTime * 1.0f;
    static bool movingLeft = false;
    bool curMovingLeft = movingLeft;
    for(std::unordered_map<std::string, GameObject*>::iterator iter = loadedObjects.begin(); iter != loadedObjects.end(); iter++) {
        if(iter->first != "player") { // Update the enemy game object
            // movingLeft = shouldGroupMoveLeft(iter->second);
            if(leftHitWall(iter->second)) {
                movingLeft = false; 
            } else if(rightHitWall(iter->second)) {
                movingLeft = true; 
            }
            if(!curMovingLeft) {
                iter->second->moveRight(m_x_pos);
            } else {
                iter->second->moveLeft(m_x_pos);
            }
        }
    }
    
    // Move the player bullets up
    for(size_t i = 0; i < playerBullets.size(); ++i) {
        float bulletYMovement = elapsedTime * 1.0 * playerBullets[i]->GetYDirection();
        playerBullets[i]->moveVertical(bulletYMovement);
    }

    // Check if any of the player bullets are now offscreen
    for(size_t i = 0; i < playerBullets.size(); ++i) {
        // Get the Position of the player bullets
        float yPosition = playerBullets[i]->getYPosition();
        if (yPosition > 3.55f) {
            delete playerBullets[i];
            playerBullets[i] = nullptr;
            playerBullets.erase(playerBullets.begin() + i);
        }
    } 

    // Randomly select an enemy to fire a bullet
    if (enemyShouldFire(enemyBullets.size())) {
        int firingEnemyNumber = rand()%NUM_ENEMIES;
        std::string firingEnemyID = "enemy" + std::to_string(firingEnemyNumber);
        GameObject* firingEnemy = GetObject(firingEnemyID);
        if (firingEnemy != nullptr) {
            Texture* bulletTexture = LoadTexture(RESOURCE_FOLDER"RedPaddle.png");
            GameObject* newEnemyBullet = new GameObject("bullet", bulletTexture);
            newEnemyBullet->Scale(.01f, .001f, 0.0f);
            newEnemyBullet->SetPosition(firingEnemy->getXPosition(), firingEnemy->getYPosition());
            newEnemyBullet->SetYDirection(-1.0f);
            enemyBullets.push_back(newEnemyBullet);
        }
    }

    // Move the enemy bullets down
    for(size_t i = 0; i < enemyBullets.size(); ++i) {
        float bulletYMovement = elapsedTime * 1.0 * enemyBullets[i] ->GetYDirection();
        enemyBullets[i] -> moveVertical(bulletYMovement);
    }

    // Check if any of the enemy bullets are now offscreen
    for(size_t i = 0; i < enemyBullets.size(); ++i) {
        float yPosition = enemyBullets[i]->getYPosition();
        if (yPosition < -3.55f) {
            delete enemyBullets[i];
            enemyBullets[i] = nullptr;
            enemyBullets.erase(enemyBullets.begin() + i);
        }
    }

    // Collision detection:
    // Check if player bullet collides with enemy
    for (size_t i = 0 ; i < playerBullets.size(); ++i) {
        GameObject* currentBullet = playerBullets[i];
        for (auto iter = loadedObjects.begin(); iter != loadedObjects.end(); ) {
            bool removed = false;
            if (iter->first != "player") {
                // Check if the enemy collides 
                if(currentBullet!= nullptr && iter->second!=nullptr &&currentBullet->Collides(iter->second)) {
                    delete iter->second;
                    iter->second = nullptr;
                    iter = loadedObjects.erase(iter);
                    if (currentBullet != nullptr) {
                        delete currentBullet;
                        currentBullet = nullptr;
                        playerBullets.erase(playerBullets.begin() + i);
                    }
                    removed = true;
                }
            }
            if(!removed) {
                ++iter; 
            }
        }
    }
    // Check if enemy bullet collides with player
    for (size_t i = 0 ; i < enemyBullets.size(); ++i) {
        GameObject* currentBullet = enemyBullets[i];
        GameObject* player = GetObject("player");
        if (currentBullet->Collides(player)) {
            // TODO: Consider flashing "Game Over" to screen here
            std::cerr << "Game Over!" << std::endl;    
            Cleanup();
            displayMenu = true;
            return;
        }
    }

    // Check if Game Over
    if (loadedObjects.size() == 1) {
        for(auto iter = loadedObjects.begin(); iter != loadedObjects.end(); ++iter) {
            if (iter->first == "player") {
                std::cout << "You Win!" << std::endl; 
                // TODO: Consider flashing you win to screen here
                Cleanup();
                displayMenu = true;
                return;
            } 
        }
    }
}

// Updates elapsed time since last redraw
void Game::updateElapsedTime() {
	if(!timeUpdated) {
		float ticks = (float) SDL_GetTicks()/1000.0f;
		elapsedTime = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
	}
	timeUpdated = true;
}

void Game::InputEventUpdate(SDL_Event& event) {
    if(displayMenu) {
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                std::cerr << "ENTER KEY HIT" << std::endl;
                displayMenu = false;
                Setup();
            }
        }
        return;
    }
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
            GameObject* player = GetObject("player");
            if (player != nullptr && playerBullets.size() < NUM_PLAYER_BULLETS) {
                Texture* bulletTexture = LoadTexture(RESOURCE_FOLDER"BluePaddle.png");
                GameObject* newBullet = new GameObject("bullet", bulletTexture);
                newBullet->Scale(.01f, .001f, 0.0f);
                newBullet->SetPosition(player->getXPosition(), player->getYPosition());
                newBullet->SetYDirection(1.0f);

                playerBullets.push_back(newBullet);
            }
        } 
    }

}

// Update based on any user input
void Game::EventUpdate() {
    if(displayMenu) {
        return;
    }
	updateElapsedTime();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
    // TODO: Change this to utilize ACCELERATION INSTEAD
	float m_x_pos = elapsedTime * PLAYER_MOVEMENT_SPEED;

    // Player Movement Left
	if(keys[SDL_SCANCODE_LEFT]) {
		GameObject* player = GetObject("player");
        // Ensure the Player is still on screen
		if (player != nullptr && player->getXPosition() - player->getWidth()/2.0f > -3.55f) {
             player->moveLeft(m_x_pos);
		}
	} 

    // Player Movement right
	if(keys[SDL_SCANCODE_RIGHT]) {
		GameObject* player = GetObject("player");
        // Ensure player remains on screen
		if (player != nullptr && player->getXPosition() + player->getWidth()/2.0f < 3.55f) {
             player->moveRight(m_x_pos);
		}
	}
}
// Redraw all game objects
// reset the flag representing wether or not the time has been updated already or not
void Game::Redraw(ShaderProgram& program) {
	timeUpdated = false;
	glClear(GL_COLOR_BUFFER_BIT);
    if(displayMenu) {
        Texture* text = LoadTexture(RESOURCE_FOLDER"font1.png");
        Textbox(program, text, "Space Invaders", -1.4f, 1.8f);
        Textbox(program, text, "Press ENTER to start", -1.8f, 1.6f);
        SDL_GL_SwapWindow(displayWindow);
        return;
    }
    for(auto iter : loadedObjects) {
        iter.second->draw(program);
    }
    // Draw player bullets
    for(size_t i = 0; i < playerBullets.size(); ++i) {
        playerBullets[i]->draw(program);
    }
    // Draw enemy bullets
    for(size_t i = 0; i <enemyBullets.size(); ++i) {
        enemyBullets[i]->draw(program);
    }
    // swap window
	SDL_GL_SwapWindow(displayWindow);
}

// Retrieve a GameObject based on its name (id)
GameObject* Game::GetObject(std::string name) {
	auto iter = loadedObjects.find(name);
	if(iter == loadedObjects.end()) {
		return nullptr;
	}
	return iter->second;
}

// Add a GameObject to our mapping
// Returns true if succesfully added, false otherwise
bool Game::AddObject(GameObject* object) {
    auto iter = loadedObjects.find(object->getObjectID());
    if(iter == loadedObjects.end()) {
        std::pair<std::string, GameObject*> p(object->getObjectID(), object);
        loadedObjects.insert(p);
        return true;
    }
    return false;
}

// Cleanup function that removes all GameObjects and loaded textures.
void Game::Cleanup() {
    for(std::unordered_map<std::string, Texture*>::iterator iter = loadedTextures.begin(); iter != loadedTextures.end();) {
        if(iter->second != nullptr) {
            delete iter->second;
            iter->second = nullptr;
        }
        // .erase() returns the iterator 1 past the deleted element (thereby iterating to the next element
        iter = loadedTextures.erase(iter);
    }
    loadedTextures.clear();
    for(std::unordered_map<std::string, GameObject*>::iterator iter = loadedObjects.begin(); iter != loadedObjects.end();) {
        if(iter->second != nullptr) {
            delete iter->second;
            iter->second = nullptr;
        }
        iter = loadedObjects.erase(iter);
    }
    loadedObjects.clear();
    for(std::vector<GameObject*>::iterator iter = playerBullets.begin(); iter != playerBullets.end(); ++iter) {
        if(*iter != nullptr) {
            delete *iter;
            *iter = nullptr;
        }
    }
    playerBullets.clear();

    for(std::vector<GameObject*>::iterator iter = enemyBullets.begin(); iter != enemyBullets.end(); ++iter) {
        if(*iter != nullptr) {
            delete *iter;
            *iter = nullptr;
        }
    }
    enemyBullets.clear();
}

// Destructor
Game::~Game() {
	Cleanup();
}
