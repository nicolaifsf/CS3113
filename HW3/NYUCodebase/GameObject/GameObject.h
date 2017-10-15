#include <SDL.h>
#include "../Matrix.h"
#include "../ShaderProgram.h"
#include "../Texture/Texture.hpp"
#ifndef GameObject_h
#define GameObject_h

// Representation of an Entity or GameObject in our Game
class GameObject {
public:
	// Initialize a game object giving it the texture ID
    GameObject(std::string objectID, Texture* texture);
    
    // Scale a GameObject's texture with multipliers
    void Scale(float xWidth, float yHeight, float zDepth);
    
    // Set the velocity of a GameObject
    void SetVelocity(float velocity);

    // Get the ID of the GameObject
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

    // Draw the GameObject
    void draw(ShaderProgram& program);

    // Set the Game Object's position
    void SetPosition(float x, float y);

    // Set the Game Object's x direction
    void SetXDirection(float x_dir);

    // Set the Game Object's y direction
    void SetYDirection(float y_dir);

    // Get the current X Position of the Game Object
    float getXPosition();

    // Get the current Y Position of the Game Object
    float getYPosition();

    // Get the Height of the Game Object
    float getHeight();

    // Get the Width of the Game Object
    float getWidth();

    // Get the X Direction the Game Object is traveling in
    float GetXDirection();

    // Get the Y Direction the Game Object is traveling in
    float GetYDirection();

    bool Collides(GameObject* otherObject);

private:
    Matrix projectionMatrix;
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
