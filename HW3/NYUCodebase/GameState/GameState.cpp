//
//  GameState.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/15/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "GameState.hpp"
#include <iostream>
#include "../Game/Game.hpp"
#include "../Texture/Texture.hpp"

const int NUM_ENEMIES = 25;
const int NUM_ENEMY_BULLETS = 50;
const int CHANCE_BULLET = 20;
const int NUM_PLAYER_BULLETS = 10;
const int PLAYER_MOVEMENT_SPEED = 5;

GameState::GameState(Game* game) : game(game) {

}

GameState::~GameState() {
    Cleanup();
}


void GameState::Setup() {
    // Load the sheet with all the textures from game
    Texture* enemyTextureSheet = game->LoadTexture("colony_0.png");
    
    // Player Entity
    Texture* textureSheet = game->LoadTexture("sprites_4.png");
    SheetSprite playerSprite(textureSheet->getTextureID(), 0.45f, 0.12f, 0.1f, 0.10f, 4.0f);
    player = new Entity("player", playerSprite);
    player->setPosition(0.0f, -1.6f, 0.0f);
    player->setSize(0.04f, 0.04f, 1.0f);
    
    // enemy entities
    float spaceBetween = .6f;
    float initialXPos = -3.0f;
    float lastXPos = initialXPos;
    float lastYPos = 1.5f;
    for (int x = 1; x < NUM_ENEMIES; ++x) {
        SheetSprite enemySprite(enemyTextureSheet->getTextureID(), 0.3f, 0.23f, 0.050f, 0.1f, 8.0f);
        std::string enemyID = "enemy" + std::to_string(x);
        Entity* enemyEntity = new Entity(enemyID, enemySprite);
        enemyEntity->setSize(0.060f, 0.1f, 1.0f);
        std::pair<std::string, Entity*> p(enemyID, enemyEntity);
        enemies.insert(p);
        enemyEntity->setPosition(lastXPos, lastYPos, 1.0f);
        enemyEntity->setVelocity(-1.0f, 0.0f, 0.0f);
        lastXPos += spaceBetween;
        if (x%8 == 0) {
            lastYPos += -spaceBetween;
            lastXPos = initialXPos;
        }
    }
}

bool rightHitWall(Entity* obj) {
    Vector3 objectPosition = obj->getPosition();
    Vector3 objectSize = obj->getSize();
    float rightOfObject = objectPosition.getX() + objectSize.getX()/2.0f;
    return rightOfObject > 3.55f;
}

bool leftHitWall(Entity* obj) {
    Vector3 objectPosition = obj->getPosition();
    Vector3 objectSize = obj->getSize();
    float leftOfObject = objectPosition.getX() - objectSize.getX()/2.0f;
    return leftOfObject < -3.55f;
}

bool didHitWall(Entity* obj) {
    return leftHitWall(obj) || rightHitWall(obj);
}

bool enemyShouldFire(size_t numEnemyBullets) {
    return NUM_ENEMY_BULLETS > numEnemyBullets && rand()%100 < CHANCE_BULLET;
}

void GameState::Update(float elapsedTime) {
    float m_x_pos = elapsedTime * 1.0f;
    static bool movingLeft = false;
    bool curMovingLeft = movingLeft;

    for(std::unordered_map<std::string, Entity*>::iterator iter = enemies.begin(); iter != enemies.end(); iter++) {
        if(leftHitWall(iter->second)) {
            movingLeft = false;
        } else if(rightHitWall(iter->second)) {
            movingLeft = true;
        }
        if(!curMovingLeft) {
            Vector3 velocity = iter->second->getVelocity();
            iter->second->moveHorizontal(-1.0f * m_x_pos * velocity.getX());
        } else {
            Vector3 velocity = iter->second->getVelocity();
            iter->second->moveHorizontal(1.0f * m_x_pos * velocity.getX());
        }
    }
    
    // Move the player bullets up
    for(size_t i = 0; i < playerBullets.size(); ++i) {
        Vector3 currBulletVelocity = playerBullets[i]->getVelocity();
        playerBullets[i]->moveVertical(currBulletVelocity.getY() * elapsedTime * 1.0f);
    }
    
    // Check if any of the player bullets are now offscreen
    for(size_t i = 0; i < playerBullets.size(); ++i) {
        // Get the Position of the player bullets
        Vector3 currBulletPosition = playerBullets[i]->getPosition();
        if (currBulletPosition.getY() > 3.55f) {
            delete playerBullets[i];
            playerBullets[i] = nullptr;
            playerBullets.erase(playerBullets.begin() + i);
        }
    }
    // Randomly select an enemy to fire a bullet
    if (enemyShouldFire(enemyBullets.size())) {
        int firingEnemyNumber = rand()%(NUM_ENEMIES) +1;
        std::string firingEnemyID = "enemy" + std::to_string(firingEnemyNumber);
        auto iter = enemies.find(firingEnemyID);
        if (iter != enemies.end()) {
            Entity* firingEnemy = iter->second;
            if (firingEnemy != nullptr) {
                Texture* bulletTexture = game->LoadTexture("sprites_4.png");
                SheetSprite bulletSprite(bulletTexture->getTextureID(), 0.01f, 0.0f, 0.15f, 0.4f, 1.0f);
                Entity* newEnemyBullet = new Entity("bullet", bulletSprite);
                newEnemyBullet->setSize(0.14f, 0.4f, 1.0f);
                newEnemyBullet->setVelocity(0.0f, -1.0f, 0.0f);
                Vector3 enemyPosition = firingEnemy->getPosition();
                newEnemyBullet->setPosition(enemyPosition);
                enemyBullets.push_back(newEnemyBullet);
            }
        }
    }
    
    // Move the enemy bullets down
    for(size_t i = 0; i < enemyBullets.size(); ++i) {
        Vector3 bulletDirection = enemyBullets[i]->getVelocity();
        float bulletYMovement = elapsedTime * 1.0 * bulletDirection.getY();
        enemyBullets[i] -> moveVertical(bulletYMovement);
    }
    
    // Check if any of the enemy bullets are now offscreen
    for(size_t i = 0; i < enemyBullets.size(); ++i) {
        Vector3 bulletPosition = enemyBullets[i]->getPosition();
        float yPosition = bulletPosition.getY();
        if (yPosition < -3.55f) {
            delete enemyBullets[i];
            enemyBullets[i] = nullptr;
            enemyBullets.erase(enemyBullets.begin() + i);
        }
    }
    
    // Collision detection:
    // Check if player bullet collides with enemy
    for (size_t i = 0 ; i < playerBullets.size(); ++i) {
        Entity* currentBullet = playerBullets[i];
        for (auto iter = enemies.begin(); iter != enemies.end(); ) {
            bool removed = false;
                // Check if the enemy collides
                if(currentBullet!= nullptr && iter->second!=nullptr &&currentBullet->collide(iter->second)) {
                    delete iter->second;
                    iter->second = nullptr;
                    iter = enemies.erase(iter);
                    if (currentBullet != nullptr) {
                        delete currentBullet;
                        currentBullet = nullptr;
                        playerBullets.erase(playerBullets.begin() + i);
                    }
                    removed = true;
                }
            if(!removed) {
                ++iter;
            }
        }
    }
    // Check if enemy bullet collides with player
    for (size_t i = 0 ; i < enemyBullets.size(); ++i) {
        Entity* currentBullet = enemyBullets[i];
        if (currentBullet->collide(player)) {
            std::cerr << "Game Over!" << std::endl;
            Cleanup();
            game->setGameMode(STATE_MENU);
        }
    }
    
    // Check if Game Over
    if(enemies.size() == 0) {
        std::cout << "You win!" << std::endl;
        Cleanup();
        game->setGameMode(STATE_MENU);
    }
}

void GameState::Redraw(ShaderProgram& program) {
    player->draw(program);
    for(auto iter = enemies.begin(); iter != enemies.end(); ++iter) {
        iter->second->draw(program);
    }
    for(auto iter = playerBullets.begin(); iter != playerBullets.end(); ++iter) {
        (*iter)->draw(program);
    }
    for(auto iter = enemyBullets.begin(); iter != enemyBullets.end(); ++iter) {
        (*iter)->draw(program);
    }
}
void GameState::EventUpdate(float elapsedTime) {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    // TODO: Change this to utilize ACCELERATION INSTEAD
    float m_x_pos = elapsedTime * PLAYER_MOVEMENT_SPEED;
    
    Vector3 playerPos = player->getPosition();
    Vector3 playerSz = player->getSize();
    
    // Player Movement Left
    if(keys[SDL_SCANCODE_LEFT]) {
        // Ensure the Player is still on screen

        if (player != nullptr && playerPos.getX() - playerSz.getX()/2.0f > -3.55f) {
            player->moveHorizontal(-m_x_pos);
        }
    }
    
    // Player Movement right
    if(keys[SDL_SCANCODE_RIGHT]) {
        // Ensure player remains on screen
        if (player != nullptr && playerPos.getX() + playerSz.getX()/2.0f < 3.55f) {
            player->moveHorizontal(m_x_pos);
        }
    }
}
void GameState::InputEventUpdate(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        // If player pushes space bar
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
            if (player != nullptr && playerBullets.size() < NUM_PLAYER_BULLETS) {
                Texture* bulletTexture = game->LoadTexture("sprites_4.png");
                SheetSprite bulletSprite(bulletTexture->getTextureID(), 0.2f, 0.3f, .2f, 0.2f, 1.0f);

                Entity* newBullet = new Entity("bullet", bulletSprite);
                newBullet->setVelocity(0.0f, 1.0f, 0.0f);
                newBullet->setPosition(player->getPosition());
                newBullet->setSize(0.2f, 0.2f, 1.0f);
                playerBullets.push_back(newBullet);
            }
        }
    }
}
void GameState::Cleanup() {
    // Delete enemies
    for(auto iter = enemies.begin(); iter != enemies.end(); ) {
        if (iter->second != nullptr) {
            delete iter->second;
            iter->second = nullptr;
        }
        iter = enemies.erase(iter);
    }
    // Delete enemy bullets
    for(auto iter = enemyBullets.begin(); iter != enemyBullets.end(); ++iter) {
        if( *iter != nullptr) {
            delete *iter;
            *iter = nullptr;
        }
    }
    enemyBullets.clear();
    // Delete player bullets
    for(auto iter = playerBullets.begin(); iter != playerBullets.end(); ++iter) {
        if( *iter != nullptr) {
            delete *iter;
            *iter = nullptr;
        }
    }
    playerBullets.clear();
    // Delete player
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }
}
