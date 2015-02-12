#version 430 core

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 color;

void main()
{		
    //FragColor = vec4(1.0,1.0,0.0,1.0);
	FragColor = color;
}
