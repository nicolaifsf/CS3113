#include <SDL.h>
#include "../Matrix.h"
#include "../ShaderProgram.h"
#ifndef GameObject_h
#define GameObject_h

/**
Implemented a simple GameObject class so as to not have to worry about
remembering which matrices corresponded to which images and etc
TODO: Refactor this class... Too many random constant values for my liking.
**/
class GameObject {
public:
	// Initialize a game object giving it the texture ID
    GameObject(GLuint textureID);

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
private:
	Matrix projectionMatrix;
	Matrix modelviewMatrix;
	GLuint textureID;
};

#endif /* GameObject_h */
