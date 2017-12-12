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
#include "../Tilemap/Tilemap.h"
#include <SDL_Mixer.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#define SMALL_PEN_RESOLVE_FACTOR  0.00001f
#define FRICTION_X 8.0f
#define FRICTION_Y 0.1f
#define TILE_SIZE 1.0f
#define MAX_GROUND 15


#define SCREEN_SHAKE_SPEED 30.5f
#define SCREEN_SHAKE_INTENSITY 0.4f


using namespace std;

float mapValue(float value, float srcMin, float srcMax, float dstMin, float dstMax) {
    float retVal = dstMin + ((value - srcMin)/(srcMax-srcMin) * (dstMax - dstMin));
    if(retVal < dstMin) {
        retVal = dstMin;
    }
    if(retVal > dstMax) {
        retVal = dstMax; 
    }
    return retVal;
}

int getWorldHeight(const vector<vector<int>>& worldMap) {
    return worldMap.size()-1; 
}

int getWorldWidth(const vector<vector<int>>& worldMap) {
    return worldMap[0].size()-1;
}

vector<vector<int>> generateLevel2() {
    const int LEVEL_HEIGHT = 16;
    const int LEVEL_WIDTH = 16;
    vector<vector<int>> world(LEVEL_HEIGHT, vector<int>(LEVEL_WIDTH, 0));
    world = {{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
             {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
             {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
             {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
             {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
             {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {1,16,16,16,0,0,0,0,0,0,0,0,0,0,0,0},
            {14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14}
    };
    
    return world;
}

vector<vector<int>> generateLevel1() {
    const int LEVEL_HEIGHT = 16;
    const int LEVEL_WIDTH = 16;
    vector<vector<int>> world(LEVEL_HEIGHT, vector<int>(LEVEL_WIDTH, 0)); 
    for(int i = 1; i < getWorldWidth(world); ++i) {
        int spriteIndex = rand()%MAX_GROUND;
        // int spriteIndex = 1;
        world[getWorldHeight(world)-1][i] = spriteIndex;
    }
    world[getWorldHeight(world)-1][3] = 0;
    for(int i = 5; i < getWorldWidth(world); ++i) {
        int spriteIndex = 1;
        world[getWorldHeight(world)-2][i] = spriteIndex;
    }
    for(int i = 2; i > 0; --i) {
        int spriteIndex = 1;
        world[getWorldHeight(world)-2][i] = spriteIndex;
    }
    return world;
}

void GameState::generateWorldMap(int level) {
    vector<vector<int>> world;
    switch(level) {
      case 1:
        parseFile("NYUCodebase.app/Contents/Resources/map1.txt");
        break;
      case 2:
        parseFile("NYUCodebase.app/Contents/Resources/map2.txt");
        break;
      case 3:
        parseFile("NYUCodebase.app/Contents/Resources/map3.txt");
        break;
    }
}

void worldToTileCoordinates(float worldX, float worldY, int* gridX, int* gridY) {
    *gridX = (int) (worldX / TILE_SIZE);
    *gridY = (int) (-worldY / TILE_SIZE);
}

Mix_Music* initializeMixer() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_Music* music = Mix_LoadMUS("Broke.mp3");
    return music;
}

GameState::GameState(Game* game) : game(game), music(nullptr), jumpSound(nullptr), player(nullptr), coinSound(nullptr), hurtSound(nullptr), level(nullptr), playerJumped(false) {
    screenShakeValue = 0.0f;
    saturation = 0.0f;
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

Entity* placeGroundBlock(Game* game, const vector<vector<int>>& worldMap, int x, int y) {
    Texture* groundTexture = game->LoadTexture("JnRTiles.png");
    // int spriteIndex = rand()%15;
    int spriteIndex = worldMap[x][y]-1;
    int SPRITE_COUNT_X = 18;
    int SPRITE_COUNT_Y = 1;
    float u = (float)(spriteIndex % SPRITE_COUNT_X) / (float) SPRITE_COUNT_X;
    float v = (float)(spriteIndex / SPRITE_COUNT_X) / (float) SPRITE_COUNT_Y;

    float spriteWidth=1.0f/(float)SPRITE_COUNT_X;
    float spriteHeight = 1.0f/(float)SPRITE_COUNT_Y;
    SheetSprite groundSprite(groundTexture->getTextureID(), u, v, spriteWidth, spriteHeight, 1.0f);
    Entity* groundEntity = new Entity("ground", groundSprite);
    // groundEntity->setSize(.7f, .80f, 1.0f);
    groundEntity->setSize(1.0f, 1.0f, 1.0f);
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
    coin->setSize(.5f, .8f, 1.0f);
    coin->setScale(1.0f, 1.0f, 1.0f);
    coin->setPosition(x, y, 0.0f);
    coin->setStatic(false);
    return coin;
}

// void createRandomPath(Game* game, vector<Entity*>& world, vector<Entity*>& coins, int startX, int startY,  int length) {
//     int currX = startX;
//     int currY = startY;
//     for(int i = 0; i < length; ++i) {
//         int randVal = rand()%7;
//         if(randVal == 0 || randVal == 5) {
//             world.push_back(placeGroundBlock(game, currX, currY));
//         } else if (randVal == 1) {
//             world.push_back(placeGroundBlock(game,currX, ++currY)); 
//         } else if(randVal == 2) {
//             world.push_back(placeGroundBlock(game,currX, --currY)); 
//         } else if(randVal == 3) {
//             world.push_back(placeGroundBlock(game,currX, --currY)); 
//             world.push_back(placeGroundBlock(game,currX, --currY)); 
//         } else if(randVal == 4) {
//             world.push_back(placeGroundBlock(game,currX, ++currY)); 
//             world.push_back(placeGroundBlock(game,currX, ++currY));        
//         } else if (randVal == 6) {
//             // place a coin above a single block 
//             coins.push_back(placeCoin(game, currX, currY+1));
//             world.push_back(placeGroundBlock(game, currX, currY));
//         }
//         ++currX;
//     }
// }

void GameState::Setup(int levelSelect) {
    music = initializeMixer(); 
    Mix_PlayMusic(music, -1);
    jumpSound = Mix_LoadWAV("jump.wav");
    coinSound = Mix_LoadWAV("coin.wav");
    hurtSound = Mix_LoadWAV("hurt.wav");
    landSound = Mix_LoadWAV("land.wav");
    srand(time(NULL)); 
    
    // parseFile("NYUCodebase.app/Contents/Resources/map1.txt");
    
    // Player Entity


    generateWorldMap(levelSelect);
    
    // player = new Entity("player", loadAGeorge(0, game));
    // player->setPosition(5, -getWorldHeight(worldMap)+3, 0.0f);
    // int x;
    // int y;
    // worldToTileCoordinates(3, 3, &x, &y);
    // player->setScale(1.0f, 1.0f, 1.0f);
    // player->setSize(0.4f, 0.8f, 1.0f);
    // player->setAcceleration(0.0f, 0.0f, 0.0f);
    // player->setVelocity(0.0f, 0.0f, 0.0f);
    // player->setStatic(false);


    Texture* worldSheet = game->LoadTexture("JnRTiles.png");
    int SPRITESX = 18;
    int SPRITESY = 1;

    level = new Tilemap(worldMap, worldSheet, SPRITESX, SPRITESY, TILE_SIZE);

    // coins.push_back(placeCoin(game, 8, -getWorldHeight(worldMap)+ 3));
    // coins[0]->setAcceleration(-16.0f, coins[0]->getAcceleration().getY(), coins[0]->getAcceleration().getZ());
    
    // Ground
    // world.push_back(placeGroundBlock(game, 0.0f, -3.0f));
    // world.push_back(placeGroundBlock(game, 0.0f, -4.0f));
    // world.push_back(placeGroundBlock(game, 1.0f, -5.0f));
    // world.push_back(placeGroundBlock(game, 2.0f, -4.0f));
    // world.push_back(placeGroundBlock(game, 3.0f, -4.0f));
    // world.push_back(placeGroundBlock(game, -1.0f, -3.0f));
    // world.push_back(placeGroundBlock(game, -1.0f, -2.0f));
    // world.push_back(placeGroundBlock(game, -1.0f, -1.0f));
    // world.push_back(placeGroundBlock(game, -1.0f, -0.0f));
    // world.push_back(placeGroundBlock(game, -1.0f, 1.0f));
    // for(int i = -500; i < 500; ++i) {
    //     world.push_back(placeGroundBlock(game, -2.0f, i)); 
    // }
    // for(int i = -500; i < 500; ++i) {
    //     world.push_back(placeGroundBlock(game, 204.0f, i));
    // }
    //
    // createRandomPath(game, world, coins, 4.0f, -4.0f, 200);
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

void resolveBotCollisionWorldMap(Entity* entity, const vector<vector<int>>& worldMap) {
    int gridX, gridY;
    Vector3 position = entity->getPosition();
    worldToTileCoordinates(position.getX(), position.getY() - entity->getSize().getY()/2, &gridX, &gridY);

    float ypoint = -TILE_SIZE * gridY;
    float entityBot = position.getY() - entity->getSize().getY()/2;
    float penetration = ypoint - entityBot;
    if(entityBot < ypoint) {
        entity->setPosition(position.getX(),
                             position.getY() + penetration + SMALL_PEN_RESOLVE_FACTOR,
                             position.getZ());
        Vector3 entityVelocity = entity->getVelocity();
        entity->setVelocity(entityVelocity.getX(), 0.0f, entityVelocity.getZ()); 
        // entity->setAcceleration(entity->getAcceleration().getX(), 0.0f, entity->getAcceleration().getZ());
    }
}

void resolveTopCollisionWorldMap(Entity* entity, const vector<vector<int>>& worldMap) {
    int gridX, gridY;
    Vector3 position = entity->getPosition();
    worldToTileCoordinates(position.getX(), position.getY() + entity->getSize().getY()/2, &gridX, &gridY);

    float ypoint = (-TILE_SIZE * gridY) - TILE_SIZE;
    float entityTop = position.getY() + entity->getSize().getY()/2;
    float penetration = entityTop - ypoint;
    entity->setPosition(position.getX(),
                        position.getY() - penetration - SMALL_PEN_RESOLVE_FACTOR,
                        position.getZ());
}

void resolveLeftCollisionWorldMap(Entity* entity, const vector<vector<int>>& worldMap) {
    int gridX, gridY;
    Vector3 position = entity->getPosition();
    worldToTileCoordinates(position.getX() - entity->getSize().getX()/2, position.getY(), &gridX, &gridY);

    float xpoint = (TILE_SIZE * gridX) + TILE_SIZE;
    float entityLeft = position.getX() - entity->getSize().getX()/2;
    float penetration = xpoint - entityLeft;
    entity->setPosition(position.getX() + penetration + SMALL_PEN_RESOLVE_FACTOR,
                        position.getY(),
                        position.getZ());
}

void resolveRightCollisionWorldMap(Entity* entity, const vector<vector<int>>& worldMap) {
    int gridX, gridY;
    Vector3 position = entity->getPosition();
    worldToTileCoordinates(position.getX() + entity->getSize().getX()/2, position.getY(), &gridX, &gridY);

    float xpoint = (TILE_SIZE * gridX); 
    float entityRight = position.getX() + entity->getSize().getX()/2;
    float penetration = entityRight - xpoint;
    entity->setPosition(position.getX() - penetration - SMALL_PEN_RESOLVE_FACTOR,
                        position.getY(),
                        position.getZ());
}


void Move(Entity* entity, float elapsedTime, const vector<vector<int>>& worldMap) {
    Vector3 position = entity->getPosition();

    accelAndFrictToVelocity(entity, elapsedTime);
    gravityToVelocity(entity, elapsedTime);
    
    yAxisVelToPosition(entity, elapsedTime);

    // Handle Collision with ground
    int entityBotX;
    int entityBotY;
    worldToTileCoordinates(position.getX(), 
                           position.getY() - entity->getSize().getY()/2 - SMALL_PEN_RESOLVE_FACTOR, &entityBotX, &entityBotY);
    // cout << worldMap[entityBotY][entityBotX] << endl;
    if (worldMap[entityBotY][entityBotX] != 0) {
        if(!entity->getCollidedBot()) {
            entity->setCollidedBot();
        }
        resolveBotCollisionWorldMap(entity, worldMap);
        // Vector3 entityVelocity = entity->getVelocity();
        // entity->setVelocity(entityVelocity.getX(), 0.0f, entityVelocity.getZ()); 
        // entity->setAcceleration(entity->getAcceleration().getX(), 0.0f, entity->getAcceleration().getZ());
    }

    // Handle Collision with top
    int entityTopX;
    int entityTopY;
    worldToTileCoordinates(position.getX(),
                           position.getY() + entity->getSize().getY()/2, &entityTopX, &entityTopY);
    if(worldMap[entityTopY][entityTopX] != 0) {
        entity->setCollidedTop();
        resolveTopCollisionWorldMap(entity, worldMap);
        Vector3 entityVelocity = entity->getVelocity();
        entity->setVelocity(entityVelocity.getX(), 0.0f, entityVelocity.getZ()); 
        entity->setAcceleration(entity->getAcceleration().getX(), 0.0f, entity->getAcceleration().getZ());
    }


    xAxisVelToPosition(entity, elapsedTime);

    // Handle Collision with left
    int entityLeftX;
    int entityLeftY;
    worldToTileCoordinates(position.getX() - entity->getSize().getX()/2,
                           position.getY(), &entityLeftX, &entityLeftY);
    if(worldMap[entityLeftY][entityLeftX] != 0 || entityLeftX <= 0) {
        if(!entity->getCollidedLeft()) {
            entity->setCollidedLeft();
        }
        resolveLeftCollisionWorldMap(entity, worldMap);

        Vector3 entityVelocity = entity->getVelocity();
        entity->setVelocity(0.0f, entityVelocity.getY(), entityVelocity.getZ());
        entity->setAcceleration(0.0f, entity->getAcceleration().getY(), entity->getAcceleration().getZ());
    }

    int entityRightX, entityRightY;
    worldToTileCoordinates(position.getX() + entity->getSize().getX()/2,
                           position.getY(), &entityRightX, &entityRightY);
    if(worldMap[entityRightY][entityRightX] != 0 || entityRightX > getWorldWidth(worldMap)) {
        entity->setCollidedRight();
        resolveRightCollisionWorldMap(entity, worldMap);

        Vector3 entityVelocity = entity->getVelocity();

        entity->setVelocity(0.0f, entityVelocity.getY(), entityVelocity.getZ());
        entity->setAcceleration(0.0f, entity->getAcceleration().getY(), entity->getAcceleration().getZ());
    }

}

// void manageBotCollision(entity)


void GameState::EntityUpdate(Entity* entity, float elapsedTime) {
        Vector3 position;
        Vector3 eSize;
        Vector3 velocity;

        accelAndFrictToVelocity(entity, elapsedTime);
        gravityToVelocity(entity, elapsedTime);

        // SCALE
        if(entity != player) {
            float scale_x = mapValue(fabs(entity->getVelocity().getX()), 0.0, 5.0f, 0.8f, 2.6);
            float scale_y = mapValue(fabs(entity->getVelocity().getX()), 5.0, 0.0f, 0.5, 1.0);
            entity->setScale(scale_x, scale_y, entity->getScale().getZ());
        }

        yAxisVelToPosition(entity, elapsedTime);

        // Check for Bottom Collision
        int xBot, yBot;
        position = entity->getPosition();
        eSize = entity->getSize();
        velocity = entity->getVelocity();
        worldToTileCoordinates(position.getX(), 
                               position.getY() - eSize.getY()/2,
                               // position.getY() - eSize.getY()/2 - SMALL_PEN_RESOLVE_FACTOR, 
                               &xBot, &yBot);
        if(yBot > getWorldHeight(worldMap) || worldMap[yBot][xBot] != 0) {
            entity->setCollidedBot(); 

            // Fix if entity is sinking
            if(position.getY() - eSize.getY()/2 < -TILE_SIZE * yBot) {
                float penetration = fabs((-TILE_SIZE * yBot) - (position.getY() - eSize.getY()/2));
                entity->setPosition(position.getX(), 
                                    position.getY() + penetration + SMALL_PEN_RESOLVE_FACTOR, 
                                    position.getZ());
                entity->setVelocity(velocity.getX(), 0.0f, velocity.getZ());
            }
        }

        // Check for Top Collision
        int xTop, yTop;
        position = entity->getPosition();
        eSize = entity->getSize();
        velocity = entity->getVelocity();
        worldToTileCoordinates(position.getX(),
                               position.getY() + eSize.getY()/2,
                               &xTop, &yTop);

        if(yTop < 0 || worldMap[yTop][xTop] != 0) {
            entity->setCollidedTop();
            if(position.getY() + eSize.getY()/2 >(-TILE_SIZE * yTop) - TILE_SIZE) {
                float penetration = (position.getY() + eSize.getY()/2) - ((-TILE_SIZE *yTop) - TILE_SIZE);
                entity->setPosition(position.getX(),
                                    position.getY() - penetration - SMALL_PEN_RESOLVE_FACTOR,
                                    position.getZ());
                entity->setVelocity(velocity.getX(), 0.0f, velocity.getZ());
            }
        }

        xAxisVelToPosition(entity, elapsedTime);
        int xLeft, yLeft;
        position = entity->getPosition();
        eSize = entity->getSize();
        velocity = entity->getVelocity();
        worldToTileCoordinates(position.getX() - eSize.getX()/2,
                                position.getY(),
                                &xLeft, &yLeft);

        if(xLeft < 0 || worldMap[yLeft][xLeft] != 0) {
            entity->setCollidedLeft();
            // Resolve left collision
            if(position.getX() - eSize.getX()/2 < (TILE_SIZE*xLeft) + TILE_SIZE) {
                float penetration = fabs((position.getX() - eSize.getX()/2) - ((TILE_SIZE*xLeft) + TILE_SIZE));
                entity->setPosition(position.getX() + penetration + SMALL_PEN_RESOLVE_FACTOR,
                                    position.getY(),
                                    position.getZ());
                entity->setVelocity(0.0f, velocity.getY(), velocity.getZ());
            }
        }

        int xRight, yRight;
        position = entity->getPosition();
        eSize = entity->getSize();
        velocity = entity->getVelocity();
        worldToTileCoordinates(position.getX() + eSize.getX()/2,
                               position.getY(),
                               &xRight, &yRight);
        if(xRight > getWorldWidth(worldMap) || worldMap[yRight][xRight] != 0) {
            entity->setCollidedRight();
            // Resolve right collision
            if((position.getX() + eSize.getX()/2) > (TILE_SIZE * xRight)) {
                float penetration = fabs((position.getX() + eSize.getX()/2) - (TILE_SIZE * xRight)); 
                entity->setPosition(position.getX() - penetration - SMALL_PEN_RESOLVE_FACTOR,
                                    position.getY(),
                                    position.getZ());
                entity->setVelocity(0.0f, velocity.getY(), velocity.getZ());
            }
        }
}

bool outOfBounds(int x, int y, const vector<vector<int>>& worldMap) {
       return(x > getWorldWidth(worldMap) || y > getWorldHeight(worldMap) ||
                 x < 0 || y < 0 );
}

void GameState::Update(float elapsedTime) {
    player->resetCollisionFlags();

    for(size_t i =0; i < coins.size(); ++i) {
        coins[i]->resetCollisionFlags(); 
    }
    
    EntityUpdate(player, elapsedTime);


    // Shake the screen if jump lands
    static float secspassed = 0.0f;
    static bool landsoundplayed = false;
    if (playerJumped && player->getCollidedBot()) {
        screenShakeValue += elapsedTime; 
        if (!landsoundplayed) {
            Mix_PlayChannel(-1, landSound, 0);
            landsoundplayed=true;
        }
        if (secspassed >= .2f) {
            secspassed = 0.0f;
            playerJumped = false;
            landsoundplayed=false;
        }
        secspassed += elapsedTime;
    }

    // make the tilemap look trippy
    static float tilesecs = 0.0f;
    // saturation = mapValue(tilesecs, 0.0f, 1.0f, 0.0f, 1.0f); 
    saturation = mapValue(sin(tilesecs * 0.4f), 0.0f, 1.0f, 0.0f, 1.0f) ;
    tilesecs += elapsedTime;
    // saturation = 1.0f; 

    for(size_t i = 0; i < coins.size(); ++i) {
        EntityUpdate(coins[i], elapsedTime);
        getCollisionsHorizontal(player, coins[i]);
        getCollisionsVertical(player, coins[i]);
        if (player->collide(coins[i])) {
            if(player->getCollidedBot() && coins[i]->getCollidedTop()) {
                Mix_PlayChannel(-1, coinSound, 0);
                delete coins[i];
                coins[i] = nullptr;
                coins.erase(coins.begin() + i);
            } else if ( (player->getCollidedRight() || player->getCollidedLeft() ) &&
                        (coins[i]->getCollidedLeft() || coins[i]->getCollidedRight())) {
                Mix_PlayChannel(-1, hurtSound, 0); 
                cout << "Game Over" << endl;
                Cleanup();
                game->setGameMode(GAME_OVER);
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                return;
            }
        }
    }

    // Check if player is out of bounds
    int xPBot, yPBot;
    worldToTileCoordinates(player->getPosition().getX(),
                           player->getPosition().getY() - player->getSize().getY()/2,
                           &xPBot, &yPBot);

    if(yPBot+1 > getWorldHeight(worldMap)) {
        cout << "GAME OVER" << endl;
        Cleanup();
        game->setGameMode(GAME_OVER);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        return;
    }

    // Check if no more coins
    if(coins.size() == 0) {
        cout << "YOU WIN!" << endl;
        Cleanup();
        game->setGameMode(VICTORY);
        glClearColor(255.0f, 255.0f, 255.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        return;
    }

    // Coins AI
    for(size_t i = 0; i < coins.size(); ++i) {
        Vector3 coinPos = coins[i]->getPosition();
        Vector3 coinSz = coins[i]->getSize();
        Vector3 coinAcc = coins[i]->getAcceleration();
        int coinX, coinY;

        bool dirSwapped = false;
        // bot left
        worldToTileCoordinates(coinPos.getX() - coinSz.getX()/2 - SMALL_PEN_RESOLVE_FACTOR,
                               coinPos.getY() - coinSz.getY()/2 - SMALL_PEN_RESOLVE_FACTOR,
                               &coinX, &coinY);
        if(!dirSwapped && 
                (outOfBounds(coinX, coinY, worldMap) || worldMap[coinY][coinX] == 0) &&
                coins[i]->getCollidedBot() &&
                coinAcc.getX() < 0) {

                coins[i]->setVelocity(0.0f, 0.0f, 0.0f);
                coins[i]->setAcceleration(16.0f, coinAcc.getY(), coinAcc.getZ());
                dirSwapped = true;
        } 

        // bot right
        worldToTileCoordinates(coinPos.getX() + coinSz.getX()/2 + SMALL_PEN_RESOLVE_FACTOR,
                               coinPos.getY() - coinSz.getY()/2 - SMALL_PEN_RESOLVE_FACTOR,
                               &coinX, &coinY);
        if(!dirSwapped && 
                (outOfBounds(coinX, coinY, worldMap) || worldMap[coinY][coinX] == 0) &&
                coins[i]->getCollidedBot() &&
                coinAcc.getX() > 0) {
            coins[i]->setVelocity(0.0f, 0.0f, 0.0f);
            coins[i]->setAcceleration(-16.0f, coinAcc.getY(), coinAcc.getZ());
            dirSwapped = true;
        }

        // left wall
        worldToTileCoordinates(coinPos.getX() - coinSz.getX()/2 - SMALL_PEN_RESOLVE_FACTOR,
                               coinPos.getY(),
                               &coinX, &coinY);

        if(!dirSwapped &&
                (outOfBounds(coinX, coinY, worldMap) || worldMap[coinY][coinX] == 0) &&
                coins[i]->getCollidedBot() && coins[i]->getCollidedLeft() &&
                coinAcc.getX() < 0) {
            coins[i]->setVelocity(0.0f, 0.0f, 0.0f);
            coins[i]->setAcceleration(16.0f, coinAcc.getY(), coinAcc.getZ());
            dirSwapped = true;
        }
        
        // right wall
        worldToTileCoordinates(coinPos.getX() + coinSz.getX()/2 + SMALL_PEN_RESOLVE_FACTOR,
                               coinPos.getY(),
                               &coinX, &coinY);

        if(!dirSwapped && 
                (outOfBounds(coinX, coinY, worldMap) || worldMap[coinY][coinX] == 0) &&
                coins[i]->getCollidedBot() && coins[i]->getCollidedLeft() &&
                coinAcc.getX() > 0) {
            coins[i]->setVelocity(0.0f, 0.0f, 0.0f);
            coins[i]->setAcceleration(-16.0f, coinAcc.getY(), coinAcc.getZ());
            dirSwapped = true;
        }
    }
    player->setAcceleration(0.0f,0.0f,0.0f);
}


/*
void OldUpdate(float elapsedTime) {
    // Refresh
    player->resetCollisionFlags();
    for(size_t i = 0; i < coins.size(); ++i) {
        coins[i]->resetCollisionFlags();
    }
    int xPlayerBot, yPlayerBot;
    worldToTileCoordinates(player->getPosition().getX(),
                           player->getPosition().getY() - player->getSize().getY()/2,
                           &xPlayerBot, &yPlayerBot);
    if(yPlayerBot > getWorldHeight(worldMap)) {
        cout << "GAME OVER" << endl;
        Cleanup();
        game->setGameMode(GAME_OVER);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        return;
    }

    float yPos = player->getPosition().getY() - player->getSize().getY()/2 - SMALL_PEN_RESOLVE_FACTOR;
    int tY, tX;
    worldToTileCoordinates(player->getPosition().getX(), yPos, &tX, &tY);
    // cout << "y : " << tY << endl;
    if (worldMap[tY][tX] != 0) {
        if (!player->getCollidedBot()) {
            player->setCollidedBot(); 
        }
    }

    if(coins.size() == 0) {
        // cout << "YOU WIN!" << endl; 
        glClearColor(255.0f, 255.0f, 255.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }


    Move(player, elapsedTime, worldMap);
    for(size_t i = 0; i < coins.size(); ++i) {
        Move(coins[i], elapsedTime, worldMap);
        // cout << "collided?" << boolalpha << coins[i]->getCollidedLeft() << endl;
        getCollisionsHorizontal(player, coins[i]);
        getCollisionsVertical(player, coins[i]);
    }
    // if (!player->isStatic()) {
    //     Movement(player, elapsedTime);
    // }

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
    // coins AI
    // static float directX = -16.0f;
    for(size_t i = 0; i < coins.size(); ++i) {
        // Check if about to fall off platform
        Vector3 coinPosition = coins[i]->getPosition();

        int coinX, coinY;
        worldToTileCoordinates(coinPosition.getX(), coinPosition.getY(), &coinX, &coinY);

        int botXLeft;
        int botYLeft;
        worldToTileCoordinates(coinPosition.getX() - coins[i]->getSize().getX()/2 - SMALL_PEN_RESOLVE_FACTOR, 
                               coinPosition.getY() - coins[i]->getSize().getY()/2 - SMALL_PEN_RESOLVE_FACTOR, 
                               &botXLeft, &botYLeft);

        int botXRight, botYRight;
        worldToTileCoordinates(coinPosition.getX() + coins[i]->getSize().getX()/2 + SMALL_PEN_RESOLVE_FACTOR,
                               coinPosition.getY() - coins[i]->getSize().getY()/2 - SMALL_PEN_RESOLVE_FACTOR,
                               &botXRight, &botYRight);

        int dirXLeft, dirYLeft;
        worldToTileCoordinates(coinPosition.getX() - coins[i]->getSize().getX()/2 - SMALL_PEN_RESOLVE_FACTOR,
                               coinPosition.getY(), 
                               &dirXLeft, &dirYLeft);
        
        // Check if theres a gap or edge of world
        // Check gap left
        if((worldMap[botYLeft][botXLeft] == 0 || botYLeft < 0) && coins[i]->getCollidedBot() && coins[i]->getAcceleration().getX() < 0) {
            coins[i]->setVelocity(0.0f, 0.0f, 0.0f);
            coins[i]->setAcceleration(16.0f, coins[i]->getAcceleration().getY(), coins[i]->getAcceleration().getZ());
        // Check gap right
        } else if((worldMap[botYRight][botXRight] == 0 || botXRight > getWorldWidth(worldMap)) && coins[i]->getCollidedBot() && coins[i]->getAcceleration().getX() > 0) {
            coins[i]->setVelocity(0.0f, 0.0f, 0.0f);
            coins[i]->setAcceleration(-16.0f, coins[i]->getAcceleration().getY(), coins[i]->getAcceleration().getZ());
        } 

        // Check if colliding with wall
       if(coins[i]->getCollidedLeft()) {
            coins[i]->setAcceleration(16.0f, coins[i]->getAcceleration().getY(), coins[i]->getAcceleration().getZ()); 
       } else if (coins[i]->getCollidedRight()) {
            coins[i]->setAcceleration(-16.0f, coins[i]->getAcceleration().getY(), coins[i]->getAcceleration().getZ()); 
       }
    }
    player->setAcceleration(0.0f, 0.0f, 0.0f);
}
*/


void GameState::Redraw(ShaderProgram& program) {
    Matrix playerView;
    playerView.Identity();
    playerView.SetPosition(player->getPosition().getX(),
                            player->getPosition().getY(),
                            player->getPosition().getZ());
    playerView.Translate(0.0f, sin(screenShakeValue * SCREEN_SHAKE_SPEED) * SCREEN_SHAKE_INTENSITY, 0.0f);

    level->draw(program, playerView, saturation);

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
            // if(player->getCollidedBot() && player->getAcceleration().getY() < 1.0f) {
            Vector3 playerPos = player->getPosition();
            int pX, pY;
            worldToTileCoordinates(playerPos.getX(), playerPos.getY()-player->getSize().getY()/2, &pX, &pY);
            // if(worldMap[pY][pX] != 0) {
                         
            // JUMPED!
          if(player->getCollidedBot()) {
                playerJumped = true;
                Vector3 playerAcceleration = player->getAcceleration();
                Vector3 playerVelocity = player->getVelocity();
                player->setAcceleration(playerAcceleration.getX(), 500.0f, playerAcceleration.getZ());
                // player->setVelocity(playerVelocity.getX(), 1000.0f, playerVelocity.getZ());
                Mix_PlayChannel(-1, jumpSound, 0);
            } 
        } else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            Cleanup();
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            game->setGameMode(STATE_MENU);
        }
    }
    if (event.type == SDL_KEYUP) {
        // if(event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
        //     Vector3 playerAcceleration = player->getAcceleration();
        //     player->setAcceleration(playerAcceleration.getX(), 0.0f, playerAcceleration.getZ());
        // }
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
    if (landSound != nullptr) {
        Mix_FreeChunk(landSound);
        landSound = nullptr;
    }
}

void GameState::placeEntity(string type, int placeX, int placeY) {
    // cout << "Tryna place a : " << type << endl;
    if(type == "player") {
        player = new Entity("player", loadAGeorge(0, game));
        player->setPosition(placeX, placeY, 0.0f);
        player->setScale(1.0f, 1.0f, 1.0f);
        player->setSize(0.4f, 0.8f, 1.0f);
        player->setAcceleration(0.0f, 0.0f, 0.0f);
        player->setVelocity(0.0f, 0.0f, 0.0f);
    } else if (type == "coin") {
        Entity* newCoin = placeCoin(game, placeX, placeY);
        coins.push_back(newCoin);
        newCoin->setAcceleration(-16.0f, newCoin->getAcceleration().getY(), newCoin->getAcceleration().getZ());
    }
}

void GameState::readEntityData(ifstream& stream) {
    string line;
    string type;

    while(getline(stream, line)) {
        if(line == "") { break; } 

        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);

        if(key == "type") {
            type=value;
        } else if(key =="location") {
            istringstream lineStream(value);
            string xPosition, yPosition;
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');

            float placeX = atoi(xPosition.c_str()) * TILE_SIZE;
            float placeY = atoi(yPosition.c_str()) * -TILE_SIZE;
            placeEntity(type, placeX, placeY);
        }
    }
}

void GameState::readLayerData(vector<vector<int>>& world, ifstream& stream) {
    string line;
    while(getline(stream, line)) {
        if(line =="") {break;} 
        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "data") {
            for(int y = 0; y < world.size()-1; ++y) {
                getline(stream, line);
                istringstream lineStream(line);
                string tile;

                for(int x = 0; x < world[y].size(); ++x) {
                    getline(lineStream, tile, ',');
                    int val = atoi(tile.c_str());
                    if(val > 0) {
                        world[y][x] = val;
                    } else {
                        world[y][x] = 0; 
                    }
                }
            } 
        }
    }

}

vector<vector<int>> GameState::readHeader(ifstream& stream) {
    string line;
    int mapWidth = -1;
    int mapHeight = -1;
    while(getline(stream, line)) {
        if(line=="") break; 

        istringstream sStream(line);
        string key, value;
        getline(sStream, key, '=');
        getline(sStream, value);

        if(key=="width") {
            mapWidth=atoi(value.c_str()); 
        } else if (key=="height") {
            mapHeight =atoi(value.c_str()); 
        }
    }
    vector<vector<int>> world(mapHeight, vector<int>(mapWidth, 0));
    return world;
}

void GameState::parseFile(string filename) {
    ifstream infile(filename);
    if(!infile) {
        cerr << "Failed to open: " << filename << endl; 
        exit(-1);
    }
    string line;
    while(getline(infile, line)) {
        if(line == "[header]") {
            worldMap = readHeader(infile);
        } else if (line=="[layer]") {
            readLayerData(worldMap, infile);
        } else if(line == "[Object Layer]") {
            readEntityData(infile); 
        }
    }
    // for(int i =0; i < worldMap.size(); ++i) {
    //     for(int j =0; j < worldMap[0].size(); ++j) {
    //             cout << worldMap[i][j] << " ";
    //     } 
    // }
    cout << "Parse complete" << endl;
}
