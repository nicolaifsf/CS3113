//
//  TextUtility.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/13/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "TextUtility.hpp"
#include "../ShaderProgram.h"
#include "../Matrix.h"
#include "../Texture/Texture.hpp"
#include <vector>
#include <iostream>
#include <string>

void Textbox(ShaderProgram& program, Texture* texture, std::string text, float x, float y) {
    Matrix modelViewMatrix;
    modelViewMatrix.Identity();
    modelViewMatrix.SetPosition(x, y, 1.0f);
    program.SetModelviewMatrix(modelViewMatrix);
    Matrix projectionMatrix;
    projectionMatrix.SetOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
    program.SetProjectionMatrix(projectionMatrix);

    DrawTexture(program, texture->getTextureID(), text, 0.2f, 0.01f);
}


void DrawTexture(ShaderProgram& program, GLuint fontTexture, std::string text, float size, float spacing) {
    float texture_size = 1.0/16.0f;

    std::vector<float> vertexData;
    std::vector<float> texCoordData;

    for (size_t i = 0; i < text.size(); ++i) {
        int spriteIndex = (int)text[i];

        float texture_x = (float)(spriteIndex % 16) / 16.0f;
        float texture_y = (float)(spriteIndex / 16) / 16.0f;

        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
        });
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        });
    }
    
    glUseProgram(program.programID);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program.texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
    
    
}

