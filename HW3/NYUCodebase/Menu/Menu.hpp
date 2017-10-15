//
//  Menu.hpp
//  NYUCodebase
//
//  Created by Nicolai Ferraris on 10/13/17.
//  Copyright © 2017 Ivan Safrin. All rights reserved.
//

#ifndef Menu_hpp
#define Menu_hpp
#ifdef _WINDOWS
    #include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "../ShaderProgram.h"
#include "../Texture/Texture.hpp"

class Menu {
  public:
    Menu(Texture* texture);
    void Update();
    void Redraw(ShaderProgram& program);
    void EventUpdate();
    void InputEventUpdate(SDL_Event& event);
    void Cleanup();
  private:
    Texture* textTexture;
};
#endif /* Menu_hpp */
