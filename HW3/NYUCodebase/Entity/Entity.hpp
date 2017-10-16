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

class Entity {
public:
    Entity(std::string objectID, SheetSprite sprite);
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
    Vector3 getPosition() const;
    Vector3 getVelocity() const;
    Vector3 getSize() const;
    float getRotation() const;
    void setRotation(float newRotation);
    SheetSprite getSprite() const;
    // Set the SheetSprite associated with the entity
    void setSprite(SheetSprite newSprite);
    // Redraw the entity
    void draw(ShaderProgram& program);
    // update the entity
    void update();
    // Check for any collisions
    bool collide(Entity* otherEntity);
private:
    std::string objectID;
    
    Vector3 position;
    Vector3 velocity;
    Vector3 size;
    
    float rotation;
    SheetSprite sprite;
};

#endif /* Entity_hpp */
