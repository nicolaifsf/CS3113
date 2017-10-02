#include <SDL.h>
#include "../Matrix.h"
#include "../ShaderProgram.h"
#include "../Texture/Texture.hpp"
#ifndef GameObject_h
#define GameObject_h

class GameObject {
public:
	// Initialize a game object giving it the texture ID
    GameObject(std::string objectID, Texture* texture);
    
    void Scale(float xWidth, float yHeight, float zDepth);
    
    void SetVelocity(float velocity);

    std::string getObjectID() const;

    // Move the game object horizontally
	void moveHorizontal(float units);

	// Move the game object vertically
    void moveVertical(float units);

    // Moves the game object to it's relative left
    void moveLeft(float units);

    // Moves the game object to it's relative right
    void moveRight(float units);

    // Moves the game object to it's relative up
	void moveUp(float units);

	// Moves the game object to it's relative down
    void moveDown(float units);
    
    void draw(ShaderProgram& program);

    void SetPosition(float x, float y);

    void SetXDirection(float x_dir);

    void SetYDirection(float y_dir);

    float getXPosition();

    float getYPosition();

    float getHeight();
    float getWidth();

    float GetXDirection();

    float GetYDirection();


private:
    Matrix projectionMatrix;
    Matrix modelviewMatrix;
	GLuint textureID;
    std::string objectID;

    float x;
    float y;
    float rotation;

    float width;
    float height;

    float velocity;
    float direction_x;
    float direction_y;
};

#endif /* GameObject_h */
