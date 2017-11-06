//
//  Entity.hpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/14/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include "../Vector3/Vector3.hpp"
#include "../SheetSprite/SheetSprite.hpp"
#include "../ShaderProgram.h"
#include <string>

class Matrix;

// NOT USED RIGHT NOW
enum EntityType {ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_COIN};

class Entity {
public:
    // Constructor for an Entity
    Entity(std::string objectID, SheetSprite sprite);

    // Set the type of an Entity
    void setType(EntityType type);

    // Get the type of the Entity
    EntityType getType();

    // Set the acceleration of entity with x,y and z
    void setAcceleration(float x, float y, float z);

    // Set the acceleration of entity with Vector3
    void setAcceleration(Vector3 acceleration);

    // Get the acceleration of the entity
    Vector3 getAcceleration();

    // Set the entity to be static
    void setStatic(bool stat);

    // Check if the entity is static
    bool isStatic();
    
    // Get the scale of the entity
    Vector3 getScale();
    // Set the scale of the entity
    void setScale(Vector3 newScale);
    
    // Set the scale of the entity
    void setScale(float x, float y, float z);
    // get the ID of the entity
    std::string getObjectID() const;
    // setSize sets the size of the entity with width, height, and depth
    void setSize(float width, float height, float depth);
    // setSize sets size of entity with Vector3
    void setSize(Vector3 size);
    // setVelocity sets velocity for x, y, and z directions
    void setVelocity(float x, float y, float z);
    // setVelocity sets Vector3 representing velocity of entity
    void setVelocity(Vector3 velocity);
    // moveHorizontal moves the entity horizontal units
    void moveHorizontal(float units);
    // moveVertical moves the entity vertical units
    void moveVertical(float units);
    // sets the position given x, y, and z values
    void setPosition(float x, float y, float z);
    // Sets the position using a vector3
    void setPosition(Vector3 position);
    // Get the entity's position
    Vector3 getPosition() const;
    // Get the entity's velocity
    Vector3 getVelocity() const;
    // Get the size of the entity
    Vector3 getSize() const;
    // Get the rotation of the entity
    float getRotation() const;
    // Set the rotation of the entity
    void setRotation(float newRotation);
    // Get entity's sheet sprite
    SheetSprite getSprite() const;
    // Set the SheetSprite associated with the entity
    void setSprite(SheetSprite newSprite);
    // Redraw the entity
    void draw(ShaderProgram& program, Matrix playerView);
    // update the entity
    void update();
    // Check for any collisions
    bool collide(Entity* otherEntity);

    // Reset entity's collision flags
    void resetCollisionFlags();

    // Check if top collided
    bool getCollidedTop();
    // Check if bottom bollided
    bool getCollidedBot();
    // Check if Left collided
    bool getCollidedLeft();
    // Check if right collided
    bool getCollidedRight();

    // Set the top collision flag
    void setCollidedTop();
    // Set the bottom collision flag
    void setCollidedBot();
    // Set the left collision flag
    void setCollidedLeft();
    // Set the right collision flag
    void setCollidedRight();
private:

    std::string objectID;
    
    Vector3 position;
    Vector3 velocity;
    Vector3 size;
    Vector3 scale;
    Vector3 acceleration;
    
    float rotation;
    SheetSprite sprite;

    bool isStaticEntity;

    EntityType entityType;

    bool collidedTop;
    bool collidedBot;
    bool collidedLeft;
    bool collidedRight;
};

#endif /* Entity_hpp */
