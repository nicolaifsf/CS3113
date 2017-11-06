//
//  Entity.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/14/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "Entity.hpp"
#include "../Vector3/Vector3.hpp"

void Entity::setType(EntityType type) {
    this->entityType = type;
}

EntityType Entity::getType() {
    return this->entityType;
}

void Entity::setAcceleration(float x, float y, float z) {
    this->acceleration = Vector3(x,y,z);
}

void Entity::setAcceleration(Vector3 acceleration) {
    this->acceleration = acceleration;
}

Vector3 Entity::getAcceleration() {
    return acceleration;
}

void Entity::setStatic(bool stat) {
    isStaticEntity = stat;
}

bool Entity::isStatic() {
    return isStaticEntity;
}


Entity::Entity(std::string objectID, SheetSprite sprite) : objectID(objectID), sprite(sprite) {
    this->setSize(1.0f, 1.0f, 1.0f);
}
void Entity::setSize(float width, float height, float depth) {
    this->size = Vector3(width, height, depth);
}
void Entity::setSize(Vector3 size) {
    this->size = size;
}
void Entity::setVelocity(float x, float y, float z) {
    this->velocity = Vector3(x, y, z);
}
void Entity::setVelocity(Vector3 velocity) {
    this->velocity = velocity;
}
void Entity::moveHorizontal(float units) {
    Vector3 currentPosition = this->getPosition();
    currentPosition.setX(currentPosition.getX() + units);
    this->setPosition(currentPosition);
}
void Entity::moveVertical(float units) {
    Vector3 currentPosition = this->getPosition();
    currentPosition.setY(currentPosition.getY() + units);
    this->setPosition(currentPosition);
}
void Entity::setPosition(float x, float y, float z) {
    this->position = Vector3(x, y, z);
}
void Entity::setPosition(Vector3 position) {
    this->position = position;
}
Vector3 Entity::getPosition() const {
    return this->position;
}
Vector3 Entity::getVelocity() const {
    return this->velocity;
}
Vector3 Entity::getSize() const {
    return this->size;
}
float Entity::getRotation() const {
    return this->rotation;
}
void Entity::setRotation(float newRotation) {
    this->rotation = newRotation;
}
SheetSprite Entity::getSprite() const {
    return this->sprite;
}
void Entity::setSprite(SheetSprite newSprite) {
    this->sprite = newSprite;
}

Vector3 Entity::getScale() {
    return scale;
}
void Entity::setScale(float x, float y, float z) {
    scale = Vector3(x, y, z);
}
void Entity::setScale(Vector3 newScale) {
    this->scale = newScale;
}
void Entity::draw(ShaderProgram& program, Matrix playerView) {
    Matrix modelView;
    modelView.Identity();
    Vector3 currentPosition = this->getPosition();
    modelView.SetPosition(currentPosition.getX(),
                          currentPosition.getY(),
                          currentPosition.getZ());
    Vector3 currentScale = this->getScale();
    modelView.Scale(currentScale.getX(),
                    currentScale.getY(),
                    currentScale.getZ());
    glUseProgram(program.programID);
    
    // :P
    modelView = modelView * playerView.Inverse();

    program.SetModelviewMatrix(modelView);
    
    Matrix projectionMatrix;
    // projectionMatrix.SetOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    projectionMatrix.SetOrthoProjection(-8.875, 8.875, -5.0f, 5.0f, -1.0f, 1.0f);
    // projectionMatrix.SetOrthoProjection(-88.75f, 88.75f, -50.0f, 50.0f, -1.0f, 1.0f);
    // projectionMatrix.SetOrthoProjection(-17.75f, 17.75f, -10.0f, 10.0f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    
    sprite.Draw(program);
    
}
void Entity::update() {
    
}

bool Entity::collide(Entity* otherEntity) {
    Vector3 objPosition  = this->getPosition();
    Vector3 objSize = this->getSize();
    float myTop  = objPosition.getY() + objSize.getY()/2.0f;
    float myBot = objPosition.getY() - objSize.getY()/2.0f;
    float myLeft = objPosition.getX() - objSize.getX()/2.0f;
    float myRight= objPosition.getX() + objSize.getX()/2.0f;
    
    Vector3 otherPosition = otherEntity->getPosition();
    Vector3 otherSize = otherEntity->getSize();
    float bTop = otherPosition.getY() + otherSize.getY()/2.0f;
    float bBot = otherPosition.getY() - otherSize.getY()/2.0f;
    float bLeft = otherPosition.getX() - otherSize.getX()/2.0f;
    float bRight= otherPosition.getX() + otherSize.getX()/2.0f;
    return !(myBot > bTop || myTop < bBot || myLeft > bRight || myRight < bLeft);
}

void Entity::resetCollisionFlags() {
    collidedBot = collidedLeft = collidedRight = collidedTop = false;
}

bool Entity::getCollidedTop() {
    return collidedTop;
}
bool Entity::getCollidedBot() {
    return collidedBot;
}
bool Entity::getCollidedLeft() {
    return collidedLeft;
}
bool Entity::getCollidedRight() {
    return collidedRight;
}

void Entity::setCollidedTop() {
    collidedTop = true;
}
void Entity::setCollidedBot() {
    collidedBot = true;
}
void Entity::setCollidedLeft() {
    collidedLeft = true;
}
void Entity::setCollidedRight() {
    collidedRight = true;
}
