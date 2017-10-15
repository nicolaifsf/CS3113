//
//  Menu.cpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/13/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#include "Menu.hpp"
#ifdef _WINDOWS
    #include <GL/glew.h>
#endif
#include<SDL.h>
#include<SDL_opengl.h>
#include<SDL_image.h>
#include<iostream>
#include<string>
#include "../ShaderProgram.h"
#include "../Utilities/TextUtility.hpp"
#include "../Texture/Texture.hpp"
#include "../Matrix.h"

Menu::Menu(Texture* texture) {
    textTexture = texture;
}

void Menu::Redraw(ShaderProgram& program) {
    Textbox(program, textTexture, "Hello World!", 0.00f, 0.00f);
}
