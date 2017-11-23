//
//  GameState.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/15/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "GameState.hpp"
#include <iostream>
#include <cmath>
#include <ctime>
#include "../Game/Game.hpp"
#include "../Texture/Texture.hpp"
#include <SDL_Mixer.h>

#define SMALL_PEN_RESOLVE_FACTOR 0.001f
#define FRICTION_X 8.0f
#define FRICTION_Y 0.1f

using namespace std;

Mix_Music* initializeMixer() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_Music* music = Mix_LoadMUS("Broke.mp3");
    return music;
}

GameState::GameState(Game* game) : game(game), music(nullptr), jumpSound(nullptr), player(nullptr), coinSound(nullptr), hurtSound(nullptr) {
    music = initializeMixer(); 
    Mix_PlayMusic(music, -1);
    jumpSound = Mix_LoadWAV("jump.wav");
    coinSound = Mix_LoadWAV("coin.wav");
    hurtSound = Mix_LoadWAV("hurt.wav");
}

GameState::~GameState() {
    Cleanup();
}

SheetSprite loadAGeorge(int spriteIndex, Game* game) {
    Texture* textureSheet = game->LoadTexture("george_0.png");
    int SPRITE_COUNT_X = 4;
    int SPRITE_COUNT_Y = 4;
    float u = (float)(spriteIndex % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(spriteIndex / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;

    float spriteWidth=1.0f/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y;
    SheetSprite george(textureSheet->getTextureID(), u, v, spriteWidth, spriteHeight, 1.0f);
    return george;
}

Entity* placeGroundBlock(Game* game, float x, float y) {
    Texture* groundTexture = game->LoadTexture("JnRTiles.png");
    int spriteIndex = rand()%15;
    int SPRITE_COUNT_X = 18;
    int SPRITE_COUNT_Y = 1;
    float u = (float)(spriteIndex % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(spriteIndex / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;

    float spriteWidth=1.0f/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y;
    SheetSprite groundSprite(groundTexture->getTextureID(), u, v, spriteWidth, spriteHeight, 1.0f);
    Entity* groundEntity = new Entity("ground", groundSprite);
    groundEntity->setSize(.7f, .80f, 1.0f);
    groundEntity->setScale(1.0f, 1.0f, 1.0f);
    groundEntity->setPosition(x, y, 0.0f);
    groundEntity->setStatic(true);
    return groundEntity;
}

Entity* placeCoin(Game* game, float x, float y) {
    Texture* textureSheet= game->LoadTexture("JnRTiles.png");
    int spriteIndex = 17;
    int SPRITE_COUNT_X = 18;
    int SPRITE_COUNT_Y = 1;
    float u = (float)(spriteIndex % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(spriteIndex / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;

    float spriteWidth=1.0f/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y;

    SheetSprite coinSprite(textureSheet->getTextureID(), u, v, spriteWidth-0.001f, spriteHeight, 1.0f);
    Entity* coin = new Entity("coin", coinSprite);
    coin->setSize(.5f, .9f, 1.0f);
    coin->setScale(1.0f, 1.0f, 1.0f);
    coin->setPosition(x, y, 0.0f);
    coin->setStatic(false);
    return coin;
}

void createRandomPath(Game* game, vector<Entity*>& world, vector<Entity*>& coins, int startX, int startY,  int length) {
    int currX = startX;
    int currY = startY;
    for(int i = 0; i < length; ++i) {
        int randVal = rand()%7;
        if(randVal == 0 || randVal == 5) {
            world.push_back(placeGroundBlock(game, currX, currY));
        } else if (randVal == 1) {
            world.push_back(placeGroundBlock(game,currX, ++currY)); 
        } else if(randVal == 2) {
            world.push_back(placeGroundBlock(game,currX, --currY)); 
        } else if(randVal == 3) {
            world.push_back(placeGroundBlock(game,currX, --currY)); 
            world.push_back(placeGroundBlock(game,currX, --currY)); 
        } else if(randVal == 4) {
            world.push_back(placeGroundBlock(game,currX, ++currY)); 
            world.push_back(placeGroundBlock(game,currX, ++currY));        
        } else if (randVal == 6) {
            // place a coin above a single block 
            coins.push_back(placeCoin(game, currX, currY+1));
            world.push_back(placeGroundBlock(game, currX, currY));
        }
        ++currX;
    }
}

void GameState::Setup() {
    srand(time(NULL)); 
    // Player Entity
    player = new Entity("player", loadAGeorge(0, game));
    player->setPosition(0.0f, -1.6f, 0.0f);
    player->setScale(1.0f, 1.0f, 1.0f);
    player->setSize(0.4f, 0.8f, 1.0f);
    player->setAcceleration(0.0f, 0.0f, 0.0f);
    player->setVelocity(0.0f, 0.0f, 0.0f);
    player->setStatic(false);

    // Ground
    world.push_back(placeGroundBlock(game, 0.0f, -3.0f));
    world.push_back(placeGroundBlock(game, 0.0f, -4.0f));
    world.push_back(placeGroundBlock(game, 1.0f, -5.0f));
    world.push_back(placeGroundBlock(game, 2.0f, -4.0f));
    world.push_back(placeGroundBlock(game, 3.0f, -4.0f));
    world.push_back(placeGroundBlock(game, -1.0f, -3.0f));
    world.push_back(placeGroundBlock(game, -1.0f, -2.0f));
    world.push_back(placeGroundBlock(game, -1.0f, -1.0f));
    world.push_back(placeGroundBlock(game, -1.0f, -0.0f));
    world.push_back(placeGroundBlock(game, -1.0f, 1.0f));
    for(int i = -500; i < 500; ++i) {
        world.push_back(placeGroundBlock(game, -2.0f, i)); 
    }
    for(int i = -500; i < 500; ++i) {
        world.push_back(placeGroundBlock(game, 204.0f, i));
    }

    createRandomPath(game, world, coins, 4.0f, -4.0f, 200);
}

float lerp(float v0, float v1, float t) {
    return (1.0-t)*v0 + t*v1;
}

void accelAndFrictToVelocity(Entity* entity, float elapsedTime) {
    Vector3 entityVelocity = entity->getVelocity(); 
    Vector3 entityAcceleration = entity->getAcceleration();
    float xVelocity = lerp(entityVelocity.getX(), 0.0f, elapsedTime * FRICTION_X);
    float yVelocity = lerp(entityVelocity.getY(), 0.0f, elapsedTime * FRICTION_Y);

    xVelocity += entityAcceleration.getX() * elapsedTime;
    yVelocity += entityAcceleration.getY() * elapsedTime;

    entity->setVelocity(xVelocity, yVelocity, 0.0f);
}

void gravityToVelocity(Entity* entity, float elapsedTime) {
    Vector3 entityVelocity = entity->getVelocity(); 
    float yVelocity = entityVelocity.getY();
    yVelocity = entityVelocity.getY() - 9.8 * elapsedTime;
    entity->setVelocity(entityVelocity.getX(), yVelocity, entityVelocity.getZ());
}

void yAxisVelToPosition(Entity* entity, float elapsedTime) {
    Vector3 entityPosition = entity->getPosition();
    Vector3 entityVelocity = entity->getVelocity();
    float yDirection = entityVelocity.getY() * elapsedTime;
    entity->setPosition(entityPosition.getX(),
                        entityPosition.getY() + yDirection,
                        entityPosition.getZ());
}

void xAxisVelToPosition(Entity* entity, float elapsedTime) {
    Vector3 entityPosition = entity->getPosition();
    Vector3 entityVelocity = entity->getVelocity();
    float xDirection = entityVelocity.getX() * elapsedTime;
    entity->setPosition(entityPosition.getX() + xDirection,
                        entityPosition.getY(),
                        entityPosition.getZ());
}

void getCollisionsHorizontal(Entity* entityA, Entity* entityB) {
    if(!entityA->collide(entityB)) {
        return;
    }
    Vector3 aPosition = entityA->getPosition();
    Vector3 aSize = entityA->getSize();
    Vector3 bPosition = entityB->getPosition();
    Vector3 bSize = entityB->getSize();
    // Check for left collision
    if(aPosition.getX() > bPosition.getX() &&
        (aPosition.getX() - aSize.getX()/2) < (bPosition.getX() + bSize.getX()/2)) {
        entityA->setCollidedLeft(); 
        entityB->setCollidedRight();
    }
    // Check for right collision
    if(aPosition.getX() < bPosition.getX() &&
        (aPosition.getX() + aSize.getX()/2) > (bPosition.getX() - bSize.getX()/2)){
        entityA->setCollidedRight();
        entityB->setCollidedRight();
    }
}

void getCollisionsVertical(Entity* entityA, Entity* entityB) {
    if(!entityA->collide(entityB)) {
        return;
    }
    Vector3 aPosition = entityA->getPosition();
    Vector3 aSize = entityA->getSize();
    Vector3 bPosition = entityB->getPosition();
    Vector3 bSize = entityB->getSize();
    // Check for top collision
    if(aPosition.getY() > bPosition.getY() &&
        (aPosition.getY() - aSize.getY()/2) < (bPosition.getY() + bSize.getY()/2)) {
        entityA->setCollidedBot(); 
        entityB->setCollidedTop();
    } 
    // Check for bot collision
    if(aPosition.getY() < bPosition.getY() &&
        (aPosition.getY() + aSize.getY()/2) > (bPosition.getY() - bSize.getY()/2)) {
        entityA->setCollidedTop();
        entityB->setCollidedBot();
    } 

}

void resolveYCollision(Entity* entityA, Entity* entityB) {
    // Check and resolve penetration
    Vector3 entityPosition = entityA->getPosition();
    Vector3 otherObjectPosition = entityB->getPosition();
    Vector3 entityDim = entityA->getSize();
    Vector3 otherObjDim = entityB->getSize();
    // y_distance is distance between two object's center points in y direction
    float y_distance = fabs(entityPosition.getY() - otherObjectPosition.getY());
    float penetration = fabs(y_distance - entityDim.getY()/2 - otherObjDim.getY()/2);
    
    if(entityPosition.getY() > otherObjectPosition.getY()) {
        entityA->setPosition(entityPosition.getX(), 
                            entityPosition.getY() + penetration + SMALL_PEN_RESOLVE_FACTOR,
                            entityPosition.getZ());
    }

    if(entityPosition.getY() < otherObjectPosition.getY()) {
        entityA->setPosition(entityPosition.getX(), 
                            entityPosition.getY() - penetration - SMALL_PEN_RESOLVE_FACTOR,
                            entityPosition.getZ());           
    }



}

void resolveXCollision(Entity* entityA, Entity* entityB) {
    Vector3 entityPosition = entityA->getPosition();
    Vector3 otherObjectPosition = entityB->getPosition();
    Vector3 entityDim = entityA->getSize();
    Vector3 otherObjDim = entityB->getSize();

    float x_distance = fabs(entityPosition.getX() - otherObjectPosition.getX());
    float penetration = fabs(x_distance - entityDim.getX()/2 - otherObjDim.getX()/2);

    if (entityPosition.getX() > otherObjectPosition.getX() ) {
        entityA->setPosition(entityPosition.getX() + penetration + SMALL_PEN_RESOLVE_FACTOR,
                            entityPosition.getY(), 
                            entityPosition.getZ());
    } 
    if(entityPosition.getX() < otherObjectPosition.getX()) {
         entityA->setPosition(entityPosition.getX() - penetration - SMALL_PEN_RESOLVE_FACTOR,
                            entityPosition.getY(),
                            entityPosition.getZ());           
    }
}

float getHalfHeight(Entity* entity) {
    return entity->getSize().getY()/2;
}

void GameState::Movement(Entity* entity, float elapsedTime) {
    // Go through and see if collision flag is checked
    accelAndFrictToVelocity(entity, elapsedTime);
    gravityToVelocity(entity, elapsedTime);


    yAxisVelToPosition(entity, elapsedTime);
    for(size_t i = 0; i < world.size(); ++i) {
        if(entity->collide(world[i])) {
            getCollisionsVertical(entity, world[i]);
            resolveYCollision(entity, world[i]);
            // set entity's Y velocity to 0
            Vector3 entityVelocity = entity->getVelocity();
            entity->setVelocity(entityVelocity.getX(), 0.0f, entityVelocity.getZ()); 
            entity->setAcceleration(entity->getAcceleration().getX(), 0.0f, entity->getAcceleration().getZ());
        } 
    }
    xAxisVelToPosition(entity, elapsedTime); 
    for(size_t i = 0; i < world.size(); ++i) {
        if(entity->collide(world[i])) {
            getCollisionsHorizontal(entity, world[i]);
            resolveXCollision(entity, world[i]);
            Vector3 entityVelocity = entity->getVelocity();
            entity->setVelocity(0.0f, entityVelocity.getY(), entityVelocity.getZ());
            entity->setAcceleration(0.0f, entity->getAcceleration().getY(), entity->getAcceleration().getZ());

        } 
    }
    
    // Check if contacts with player
    if (entity != player && entity->collide(player)) {
        getCollisionsVertical(entity, player);
        getCollisionsHorizontal(entity, player);
    } 
    
    // Check if player contacted any coins
    if(entity == player) {
        for(size_t i = 0; i < coins.size(); ++i) {
            if (entity->collide(coins[i])) {
                getCollisionsVertical(entity, coins[i]);
                getCollisionsHorizontal(entity, coins[i]);
            } 
        } 
    }
}

void GameState::Update(float elapsedTime) {
    // Refresh
    player->resetCollisionFlags();
    for(size_t i = 0; i < coins.size(); ++i) {
        if(!coins[i]->isStatic()) {
            coins[i]->resetCollisionFlags();
        }
    }

    if(coins.size() == 0) {
        cout << "YOU WIN!" << endl; 
        glClearColor(255.0f, 255.0f, 255.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    if (!player->isStatic()) {
        Movement(player, elapsedTime);
    }

    // Check for collision with coin
    for(size_t i = 0; i < coins.size(); ++i) {
        if(player->collide(coins[i])) {
            if(player->getCollidedBot() && coins[i]->getCollidedTop()) {
                Mix_PlayChannel(-1, coinSound, 0); 
                delete coins[i];
                coins[i] = nullptr;
                coins.erase(coins.begin() + i);
                continue;
            }
            // Player got hit by coin
            if((player->getCollidedRight() || player->getCollidedLeft()) && (coins[i]->getCollidedLeft() || coins[i]->getCollidedRight())) {
                Mix_PlayChannel(-1, hurtSound, 0);
                player->setPosition(0.0f, 0.0f, 0.0f);
            }
        } 
    }

    for(size_t i = 0; i < coins.size(); ++i) {
        if(!coins[i]->isStatic()) {
            Movement(coins[i], elapsedTime); 
            int randval = rand()%4;
            int chanceMovement = rand()%100;
            if(chanceMovement < 50) {
                if(randval  == 0) {

                    coins[i]->setAcceleration(34.5f, coins[i]->getAcceleration().getY()-10.5f, coins[i]->getAcceleration().getZ());
                } else if(randval == 1) {
                    coins[i]->setAcceleration(-34.5f, coins[i]->getAcceleration().getY()-10.5f, coins[i]->getAcceleration().getZ());
                } else if(randval == 2 ) {
                    if(coins[i]->getCollidedBot() && coins[i]->getAcceleration().getY() < 10) {
                        coins[i]->setAcceleration(-36.5f, 10.5f, 0.0f); 
                    }
                } else if(randval == 3 ) {
                    if(coins[i]->getCollidedBot() && coins[i]->getAcceleration().getY() < 10) {
                        coins[i]->setAcceleration(36.5f, 10.5f, 0.0f); 
                    }
                }
            }
        }
    }
    player->setAcceleration(0.0f, 0.0f, 0.0f);
}

void GameState::Redraw(ShaderProgram& program) {
    Matrix playerView;
    playerView.Identity();
    playerView.SetPosition(player->getPosition().getX(),
                            player->getPosition().getY(),
                            player->getPosition().getZ());

    for(int i = 0; i <  world.size(); ++i) {
        world[i]->draw(program, playerView); 
    } 
    for(int i = 0; i < coins.size(); ++i) {
        coins[i]->draw(program, playerView); 
    }
    player->draw(program, playerView);
}
void GameState::EventUpdate(float elapsedTime) {
    const Uint8* keys = SDL_GetKeyboardState(NULL);
     
    static int counter = 0;
    if(keys[SDL_SCANCODE_LEFT]) {
        static int georgeIndexLeft = 1;
        if (counter %10 == 0) {
            player->setSprite(loadAGeorge(georgeIndexLeft, game));
            georgeIndexLeft += 4;
            if(georgeIndexLeft > 14) georgeIndexLeft = 1;
        }
        Vector3 playerAcceleration = player->getAcceleration();
        player->setAcceleration(playerAcceleration.getX()-15.8f, playerAcceleration.getY(), playerAcceleration.getZ());
    } 
    if (keys[SDL_SCANCODE_RIGHT]) {
        static int georgeIndexRight = 3;
        if (counter %10 == 0) {
            player->setSprite(loadAGeorge(georgeIndexRight, game));
            georgeIndexRight += 4;
            if (georgeIndexRight > 15) georgeIndexRight = 3;
        }

        Vector3 playerAcceleration = player->getAcceleration();
        player->setAcceleration(playerAcceleration.getX()+15.8f, playerAcceleration.getY(), playerAcceleration.getZ());
    }

    ++counter;
}

void GameState::InputEventUpdate(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        // If player pushes space bar
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
            if(player->getCollidedBot() && player->getAcceleration().getY() < 1.0f) {
                Vector3 playerAcceleration = player->getAcceleration();
                player->setAcceleration(playerAcceleration.getX(), 400.0f, playerAcceleration.getZ());
                Mix_PlayChannel(-1, jumpSound, 0);
            }
        }
    }
    if (event.type == SDL_KEYUP) {
        if(event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
            Vector3 playerAcceleration = player->getAcceleration();
            player->setAcceleration(playerAcceleration.getX(), 0.0f, playerAcceleration.getZ());
        }
        if(event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
            player->setSprite(loadAGeorge(1, game));
            Vector3 playerAcceleration = player->getAcceleration();
            player->setAcceleration(0.0f, playerAcceleration.getY(), playerAcceleration.getZ());
        } 
        if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
            player->setSprite(loadAGeorge(3, game));
            Vector3 playerVelocity = player->getVelocity();
            player->setAcceleration(0.0f, playerVelocity.getY(), playerVelocity.getZ());
        } 
    }
}
void GameState::Cleanup() {
    // Delete player
    if (player != nullptr) {
        delete player;
        player = nullptr;
    }
    for(size_t i = 0; i < coins.size(); ++i) {
        if(coins[i] != nullptr) {
            delete coins[i];
            coins[i] = nullptr;
        } 
    }
    for(size_t i = 0; i < world.size(); ++i) {
        if(world[i] != nullptr) {
            delete world[i];
            world[i] = nullptr;
        } 
    }
    if (music != nullptr)  {
        Mix_FreeMusic(music);
        music = nullptr;
    }
    if (jumpSound != nullptr) {
        Mix_FreeChunk(jumpSound); 
        jumpSound = nullptr;
    }
    if (coinSound != nullptr) {
        Mix_FreeChunk(coinSound);
        coinSound = nullptr;
    }
    if (hurtSound != nullptr) {
        Mix_FreeChunk(hurtSound);
        hurtSound = nullptr;
    }
}
