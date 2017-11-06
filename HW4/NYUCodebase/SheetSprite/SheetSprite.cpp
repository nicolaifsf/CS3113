//
//  SheetSprite.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/14/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "SheetSprite.hpp"
SheetSprite::SheetSprite() : textureID(-1), u(0.0f), v(0.0f), width(0.0f), height(0.0f), size(0.0f) {}
SheetSprite::SheetSprite(GLuint textureID, float u, float v, float width, float height, float size):textureID(textureID), u(u), v(v), width(width), height(height), size(size) {}

void SheetSprite::Draw(ShaderProgram& program) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    GLfloat texCoords[] {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,
        u+width, v+height
    };
    
    // float aspect = width/height;
    float aspect = 1.0f;
    float vertices[] = {
        -0.5f * size * aspect, -0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, 0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, -0.5f * size,
        0.5f * size * aspect, -0.5f * size
    };
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}
