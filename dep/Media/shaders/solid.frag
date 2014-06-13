#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 color;
uniform float overrideAlpha;

void main()
{		
	FragColor = color * overrideAlpha;
}
