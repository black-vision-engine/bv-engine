#version 430 core

#define FRAG_COLOR		0

layout( location = FRAG_COLOR, index = 0 ) out vec4 Color;

in block
{
	vec2 Texcoord;
} In;

uniform sampler2DArray tex;
layout ( location = 1 ) uniform int layer;

void main()
{
	vec3 texCoord = vec3( In.Texcoord.xy, float( layer ) );
	Color = vec4( texture( tex, texCoord ).xyz, 1 );
}