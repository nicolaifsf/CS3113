#include "GameObject.h"

GameObject::GameObject(std::string objectID, Texture* texture) : objectID(objectID), textureID(texture->getTextureID()) {
    projectionMatrix.SetOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    width = texture->getWidth(); 
    height = texture->getHeight();
    x = 0.0f;
    y = 0.0f;
    direction_x = 0.0f;
    direction_y = 0.0f;
}


void GameObject::SetVelocity(float velocity) {
    velocity = velocity;
}

std::string GameObject::getObjectID() const {
    return objectID;
}

void GameObject::moveHorizontal(float units) {
    x += units;
}

void GameObject::moveVertical(float units) {
    y += units;
}

void GameObject::moveLeft(float units) {
    moveHorizontal(-units);
}

void GameObject::moveRight(float units) {
    moveHorizontal(units);
}

void GameObject::moveUp(float units) {
    moveVertical(units);
}

void GameObject::moveDown(float units){
    moveVertical(-units); 
}

void GameObject::SetPosition(float xPos, float yPos) {
    x = xPos;
    y = yPos;
}

void GameObject::Scale(float xWidth, float yHeight, float zDepth) {
    width *= xWidth;
    height *= yHeight;
}

void GameObject::draw(ShaderProgram& program) {
    Matrix tMatrix;
    tMatrix.Identity();
    tMatrix.SetPosition(x,y, 1.0);
    tMatrix.Scale(width, height, 1.0);
    glUseProgram(program.programID);

    program.SetModelviewMatrix(tMatrix);
    program.SetProjectionMatrix(projectionMatrix);
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute); 
}

float GameObject::getXPosition() {
    return x;
}

float GameObject::getYPosition() {
    return y;
}

float GameObject::getHeight() {
    return height;
}

float GameObject::getWidth() {
    return width; 
}

void GameObject::SetXDirection(float x_dir) {
    direction_x = x_dir;
}

void GameObject::SetYDirection(float y_dir) {
    direction_y = y_dir;
}

float GameObject::GetXDirection() {
    return direction_x;
}

float GameObject::GetYDirection() {
    return direction_y;
}