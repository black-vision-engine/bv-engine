#version 430 core

#define FRAG_COLOR		0

layout( location = FRAG_COLOR, index = 0 ) out vec4 Color;

in block
{
	vec3 Texcoord;
} In;

uniform samplerCubeArray tex;
layout ( location = 1 ) uniform int layer;

void main()
{
	vec4 texCoord = vec4( In.Texcoord, float( layer ) );
	Color = vec4( texture( tex, texCoord ).xyz, 1 );
}