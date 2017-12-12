#include "Tilemap.h"
#include <vector>
#include "../Texture/Texture.hpp"
#include "../ShaderProgram.h"
#include <iostream>

using namespace std;

Tilemap::Tilemap(std::vector< std::vector<int> >& map, Texture* texture, int spritesX, int spritesY, float tileSize) : texture(texture), worldMap(map), spriteCountX(spritesX), spriteCountY(spritesY), tileSize(tileSize) {
    levelWidth = map[0].size();
    levelHeight = map.size();
} 

void Tilemap::draw(ShaderProgram& program, Matrix playerView, float saturation) {
    Matrix modelView;
    modelView.Identity();
    glUseProgram(program.programID);
    modelView.SetPosition(0,0,0);
    modelView.Scale(1,1,1);
    modelView = modelView * playerView.Inverse();

    program.SetModelviewMatrix(modelView);

    program.SetSaturation(saturation);

    Matrix projectionMatrix;
    projectionMatrix.SetOrthoProjection(-8.875, 8.875, -5.0f, 5.0f, -1.0f, 1.0f);
    program.SetProjectionMatrix(projectionMatrix);

    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

    for(int y = 0; y < levelHeight; ++y) {
        for(int x = 0; x < levelWidth; ++x) {
            if(worldMap[y][x] != 0) {
                float u = (float)(((int) worldMap[y][x]-1) % spriteCountX) / (float) spriteCountX;
                float v = (float)(((int) worldMap[y][x]-1) / spriteCountY) / (float) spriteCountY;

                float spriteWidth = 1.0f/(float)spriteCountX;
                float spriteHeight = 1.0f/(float)spriteCountY;

                vertexData.insert(vertexData.end(), {
                   tileSize * x, -tileSize * y,
                   tileSize * x, (-tileSize * y)-tileSize,
                   (tileSize * x) + tileSize, (-tileSize * y)-tileSize,

                   tileSize * x, -tileSize * y,
                   (tileSize * x) + tileSize, (-tileSize * y)-tileSize,
                   (tileSize * x) + tileSize, -tileSize * y
                    });

                texCoordData.insert(texCoordData.end(), {
                      u, v,
                      u, v+spriteHeight,
                      u+spriteWidth, v+spriteHeight,

                      u, v,
                      u+spriteWidth, v+spriteHeight,
                      u+spriteWidth, v
                    });
            }
        } 
    }
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0,texCoordData.data());
    glEnableVertexAttribArray(program.texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6*(levelHeight+1)*(levelWidth+1));

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}

