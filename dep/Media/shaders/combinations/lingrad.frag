#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 color1;

in vec2 uvCoord;

void main()
{		
	FragColor = color1 * uvCoord.x;
}
