//
//  TextUtility.hpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/13/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef TextUtility_hpp
#define TextUtility_hpp
#include "../ShaderProgram.h"
#include "../Texture/Texture.hpp"
#include<string>

void DrawTexture(ShaderProgram& program, GLuint fontTexture, std::string text, float size, float spacing);
void Textbox(ShaderProgram& program, Texture* texture, std::string text, float x, float y);

#endif /* TextUtility_hpp */
