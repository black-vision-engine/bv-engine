#version 430 core

#define FRAG_COLOR		0

layout( location = FRAG_COLOR, index = 0 ) out vec4 Color;

in block
{
	vec3 Texcoord;
} In;

uniform samplerCube tex;

void main()
{
	Color = vec4( texture( tex, In.Texcoord ).xyz, 1 );
}