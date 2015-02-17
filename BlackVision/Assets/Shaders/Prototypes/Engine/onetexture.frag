#version 430 core

layout (location = 0) out vec4 FragColor;

uniform sampler2D Tex;
uniform float alpha;

in vec2 uvCoord;

void main()
{
	vec4 col = texture( Tex, uvCoord );
	FragColor = col * alpha; 
	//FragColor = col * alpha * col.a;  //This one uses premultiplied RGB values
}
