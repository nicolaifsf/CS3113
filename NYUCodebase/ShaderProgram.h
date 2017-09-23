#pragma once

#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Matrix.h"

class ShaderProgram {
    public:
        ShaderProgram(const char *vertexShaderFile, const char *fragmentShaderFile);
        ~ShaderProgram();
    
        void SetModelviewMatrix(const Matrix &matrix);
        void SetProjectionMatrix(const Matrix &matrix);
    
        GLuint LoadShaderFromString(const std::string &shaderContents, GLenum type);
        GLuint LoadShaderFromFile(const std::string &shaderFile, GLenum type);
    
        GLuint programID;
    
        GLuint projectionMatrixUniform;
        GLuint modelviewMatrixUniform;
    
        GLuint positionAttribute;
        GLuint texCoordAttribute;
    
        GLuint vertexShader;
        GLuint fragmentShader;
};
