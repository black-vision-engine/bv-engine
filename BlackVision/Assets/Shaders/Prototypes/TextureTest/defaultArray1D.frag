#version 430 core

#define FRAG_COLOR		0

layout( location = FRAG_COLOR, index = 0 ) out vec4 Color;

in block
{
	vec2 Texcoord;
} In;

uniform sampler1DArray tex;
layout ( location = 1 ) uniform int layer;

void main()
{
	vec2 texCoord = vec2( In.Texcoord.x, float( layer ) );
	Color = vec4( texture( tex, texCoord ).xyz, 1 );
}