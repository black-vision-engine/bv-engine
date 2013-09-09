#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 solidColor;

in float depth;

void main()
{		
	FragColor = solidColor * depth;
}
