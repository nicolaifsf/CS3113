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
#include <string>
#include <iostream>
#include <cassert>

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
    #define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

const float PADDLE_MOVEMENT_SPEED = 5.0f;
const float BALL_MOVEMENT_SPEED = 1.0f;

Game::Game(int width, int height, std::string windowName) : width(width), height(height) {
	PreSetup(windowName);
	Setup();
    /**General Setup**/
    lastFrameTicks = 0.0f;
    timeUpdated = false;
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
	GameObject* redPaddle = NewObject("redPaddle", RESOURCE_FOLDER"RedPaddle.png");
	redPaddle->SetPosition(-3.40f, 0.0f);
	redPaddle->Scale(.01f, .01f, 0.0f);

	GameObject* bluePaddle = NewObject("bluePaddle", RESOURCE_FOLDER"BluePaddle.png");
	bluePaddle->Scale(.01f, .01f, 0.0f);
	bluePaddle->SetPosition(3.40f, 0.0f);
    
    GameObject* ball = NewObject("ball", RESOURCE_FOLDER"Ball.png");
    ball->Scale(.01f, .01f, .00f);
    srand(time(NULL));
    ball->SetXDirection(cos(rand()/1000.0f));
    ball->SetYDirection(sin(rand()/1000.0f));
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

// Any updates that are not based on user input.
// Also we perform the collision detection here
void Game::Update() {
	updateElapsedTime();
    // Determine where the ball should move to
    GameObject* ball = GetObject("ball");
    if(ball == nullptr) {
        std::cerr << "Ball not properly loaded!" << std::endl;
        assert(false);
    }
   	float ballxmovement = elapsedTime* BALL_MOVEMENT_SPEED *ball->GetXDirection();
    float ballymovement = elapsedTime * BALL_MOVEMENT_SPEED *ball->GetYDirection();

    float botOfBall = ball->getYPosition() - ball->getHeight()/2.0f;
    float topOfBall = ball->getYPosition() + ball->getHeight()/2.0f;
    
    // If ball crashes into top or bottom wall, reverse the y direction
    if((botOfBall < -2.0f && ball->GetYDirection() < 0.0f)||
   		(topOfBall > 2.0f && ball->GetYDirection() > 0.0f))  {
        float ydir = ball->GetYDirection();
        ball->SetYDirection(ydir * -1.0f);
    }
    
    // Check if ball collides with paddle
    float leftOfBall = ball->getXPosition() - ball->getWidth()/2.0f;
    float rightOfBall = ball->getXPosition() + ball->getWidth()/2.0f;

    GameObject* red = GetObject("redPaddle");
    if (red == nullptr) {
        std::cerr << "Red Paddle not properly loaded!" << std::endl;
        assert(false);
    }

    float topOfRed = red->getYPosition() + red->getHeight()/2.0f;
    float botOfRed = red->getYPosition() - red->getHeight()/2.0f;
    float rightOfRed = red->getXPosition() + red->getWidth()/2.0f;
	float leftOfRed = red->getXPosition() - red->getWidth()/2.0f;


    // Check for Red paddle collision
    if (leftOfBall < rightOfRed && ball->GetXDirection() < 0.0f
    	&& topOfRed > topOfBall
    	&& botOfRed < botOfBall
    	&& leftOfRed < leftOfBall) { // needs more cases
    	float xdir = ball->GetXDirection();
    	ball->SetXDirection(xdir * -1.0f);
    }
    
    GameObject* blue = GetObject("bluePaddle");
    if (blue == nullptr) {
        std::cerr << "Blue Paddle not properly loaded!" << std::endl;
        assert(false);
    }
    float topOfBlue = blue->getYPosition() + blue->getHeight()/2.0f;
    float botOfBlue = blue->getYPosition() - blue->getHeight()/2.0f;
    float leftOfBlue = blue->getXPosition() - blue->getWidth()/2.0f;
    float rightOfBlue = blue->getXPosition() + blue->getWidth()/2.0f;
    
    // Check for Blue paddle collision
    if (rightOfBall > leftOfBlue && ball->GetXDirection() > 0.0f
        && topOfBlue > topOfBall
        && botOfBlue < botOfBall
        && rightOfBlue > rightOfBall) {
        float xdir = ball->GetXDirection();
        ball->SetXDirection(xdir * -1.0f);
    }
    
    // Check for Blue win
    if (ball->getXPosition() < leftOfRed) {
        std::cout<< "Blue scores!" << std::endl;
        ball->SetPosition(0.0f, 0.0f);
        srand(time(NULL));
        ball->SetXDirection(cos(rand()/1000.0f));
        ball->SetYDirection(sin(rand()/1000.0f));
        flash(displayWindow, 0.0f,0.0f,1.0f);
    }
    // Check for Red win
    if (ball->getXPosition() > rightOfBlue) {
        std::cout << "Red Scores!" << std::endl;
        ball->SetPosition(0.0f, 0.0f);
        srand(time(NULL));
        ball->SetXDirection(cos(rand()/1000.0f));
        ball->SetYDirection(sin(rand()/1000.0f));
        flash(displayWindow, 1.0f,0.0f,0.0f);
    }

    // Move the ball
    ball->moveHorizontal(ballxmovement);
    ball->moveVertical(ballymovement);
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

// Update based on any user input
void Game::EventUpdate() {
	updateElapsedTime();
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	float m_y_pos = elapsedTime * PADDLE_MOVEMENT_SPEED;

    // Red paddle movement down
	if(keys[SDL_SCANCODE_S]) {
		GameObject* red = GetObject("redPaddle");
        // Ensure the paddle is still on screen
		if (red != nullptr && red->getYPosition() - red->getHeight()/2.0f > -2.0f) {
             red->moveDown(m_y_pos);
		}
	} 

    // Red paddle movement up
	if(keys[SDL_SCANCODE_W]) {
		GameObject* red = GetObject("redPaddle");
        // Ensure paddle remains on screen
		if (red != nullptr && red->getYPosition() + red->getHeight()/2.0f < 2.0f) {
             red->moveUp(m_y_pos);
		}
	}

    // Blue paddle movement down
	if(keys[SDL_SCANCODE_DOWN]) {
		GameObject* blue = GetObject("bluePaddle");
        // Ensure paddle remains on screen
		if (blue != nullptr && blue->getYPosition() - blue->getHeight()/2.0f > -2.0f) {
			blue->moveDown(m_y_pos);
		}
	} 

    // Blue paddle movement up
	if(keys[SDL_SCANCODE_UP]) {
		GameObject* blue = GetObject("bluePaddle");
        // Ensure paddle remains on screen
		if (blue != nullptr && blue->getYPosition() + blue->getHeight()/2.0f < 2.0f) {
			blue->moveUp(m_y_pos);
		}
	}
}

// Redraw all game objects
// reset the flag representing wether or not the time has been updated already or not
void Game::Redraw(ShaderProgram& program) {
	timeUpdated = false;
	glClear(GL_COLOR_BUFFER_BIT);
    for(auto iter : loadedObjects) {
		iter.second->draw(program);
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
        }
        // .erase() returns the iterator 1 past the deleted element (thereby iterating to the next element
        iter = loadedTextures.erase(iter);
    }
    for(std::unordered_map<std::string, GameObject*>::iterator iter = loadedObjects.begin(); iter != loadedObjects.end();) {
        if(iter->second != nullptr) {
            delete iter->second;
        }
        iter = loadedObjects.erase(iter);
    }	
}

// Destructor
Game::~Game() {
	Cleanup();
}
