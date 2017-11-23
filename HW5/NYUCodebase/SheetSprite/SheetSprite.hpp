//
//  SheetSprite.hpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/14/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef SheetSprite_hpp
#define SheetSprite_hpp
#include "../ShaderProgram.h"

class SheetSprite {
public:
    SheetSprite();
    SheetSprite(GLuint textureID, float u, float v, float width, float height, float size);
    void Draw(ShaderProgram& program);
    
private:
    float size;
    GLuint textureID;
    float u;
    float v;
    float width;
    float height;
};
#endif /* SheetSprite_hpp */
