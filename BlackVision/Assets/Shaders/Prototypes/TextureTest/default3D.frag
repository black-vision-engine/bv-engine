#version 430 core

#define FRAG_COLOR		0

layout( location = FRAG_COLOR, index = 0 ) out vec4 Color;

in block
{
	vec2 Texcoord;
} In;

uniform sampler3D tex;
layout ( location = 1 ) uniform int depth;

void main()
{
	vec3 texCoord = vec3( In.Texcoord, depth );
	Color = vec4( texture( tex, texCoord ).xyz, 1 );
}