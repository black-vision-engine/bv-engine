#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 color1;

in vec2 uvCoord;

void main()
{		
	vec4 vec = vec4(1.0, 0.0, 0.0, 1.0);
	FragColor = vec * uvCoord.x;
//	FragColor = vec;
}
