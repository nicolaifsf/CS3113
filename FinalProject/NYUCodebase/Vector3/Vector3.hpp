//
//  Vector3.hpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/14/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef Vector3_hpp
#define Vector3_hpp

#include <stdio.h>
class Vector3 {
public:
    Vector3();
    Vector3(float x, float y, float z);
    float getX() const;
    float getY() const;
    float getZ() const;
    void setX(float newX);
    void setY(float newY);
    void setZ(float newZ);
private:
    float x;
    float y;
    float z;
};

#endif /* Vector3_hpp */
