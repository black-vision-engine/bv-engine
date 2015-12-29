#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D 	Tex0;

uniform vec4			color;
uniform vec2			shift;
uniform float			blurSize;

void main()
{
	vec4 col = texture( Tex0, uvCoord );
    
	FragColor = col;
}