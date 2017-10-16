//
//  Vector3.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/14/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "Vector3.hpp"

Vector3::Vector3(): x(0.0f), y(0.0f), z(0.0f) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z){}
float Vector3::getX() const {
    return x;
}
float Vector3::getY() const {
    return y;
}
float Vector3::getZ() const {
    return z;
}
void Vector3::setX(float newX) {
    x = newX;
}
void Vector3::setY(float newY) {
    y = newY;
}
void Vector3::setZ(float newZ) {
    z = newZ;
}

