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
    std::string getObjectID() const;
    void setSize(float width, float height, float depth);
    void setSize(Vector3 size);
    void setVelocity(float x, float y, float z);
    void setVelocity(Vector3 velocity);
    void moveHorizontal(float units);
    void moveVertical(float units);
    void setPosition(float x, float y, float z);
    void setPosition(Vector3 position);
    Vector3 getPosition() const;
    Vector3 getVelocity() const;
    Vector3 getSize() const;
    float getRotation() const;
    void setRotation(float newRotation);
    SheetSprite getSprite() const;
    void setSprite(SheetSprite newSprite);
    void draw(ShaderProgram& program);
    void update();
    bool collide(Entity* otherEntity);
    void render(ShaderProgram& program);
private:
    std::string objectID;
    
    Vector3 position;
    Vector3 velocity;
    Vector3 size;
    
    float rotation;
    SheetSprite sprite;
};

#endif /* Entity_hpp */
