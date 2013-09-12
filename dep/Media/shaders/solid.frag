#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 solidColor;

void main()
{		
	FragColor = solidColor;
}
