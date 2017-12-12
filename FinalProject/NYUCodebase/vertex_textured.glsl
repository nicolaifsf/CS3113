attribute vec4 position;
attribute vec2 texCoord;

uniform mat4 modelviewMatrix;
uniform mat4 projectionMatrix;

varying vec2 texCoordVar;

void main()
{
    texCoordVar = texCoord;
	gl_Position = projectionMatrix * modelviewMatrix  * position;
}
