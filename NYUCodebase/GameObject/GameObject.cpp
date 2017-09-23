#include "GameObject.h"

// #ifndef STB_IMAGE_IMPLEMENTATION
//     #define STB_IMAGE_IMPLEMENTATION
//     #include "../stb_image.h"
// #endif

GameObject::GameObject(GLuint textureID) : textureID(textureID) {
    projectionMatrix.SetOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);

}

void GameObject::moveHorizontal(float units) {
    modelviewMatrix.Translate(units, 0.0f, 0.0f);
}

void GameObject::moveVertical(float units) {
    modelviewMatrix.Translate(0.0f, units, 0.0f);
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

// TODO: Add the capability to SCALE the image. Remove the vertices and texcoords constants, and instead
// utilize the width and height from LoadTexture
// Would be nice if LoadTexture was part of a Game class or something.
// then what we could do is when you want to get a specific texture, can do a name lookup and get the ID
// as well as any useful metadata, like the texture's actual width and height
// Then we can apply scaling factors onto that if we would like.
void GameObject::draw(ShaderProgram& program) {

    glUseProgram(program.programID);
    program.SetModelviewMatrix(modelviewMatrix);
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