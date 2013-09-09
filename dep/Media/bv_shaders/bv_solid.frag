#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 colorSolid;

void main()
{	
	FragColor = colorSolid;
}