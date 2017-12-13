#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include "../Texture/Texture.hpp"
#include "../ShaderProgram.h"
#include "../Matrix.h"

class Tilemap {
public:
    Tilemap(std::vector< std::vector<int> >& map, Texture* texture, int spritesX, int spritesY, float tileSize); 

    void draw(ShaderProgram& program, Matrix playerView, float saturation);

private:
    int levelWidth;
    int levelHeight;
    int spriteCountX;
    int spriteCountY;
    float tileSize;
    std::vector< std::vector<int> > worldMap;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    Texture* texture;
};
#endif
