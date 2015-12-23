#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D 	Tex0;

uniform int			channelMask;

void main()
{
	vec4 col = texture( Tex0, uvCoord );
	
	int rIdx = ( channelMask >> 0 ) & 0x3;
	int gIdx = ( channelMask >> 2 ) & 0x3;
	int bIdx = ( channelMask >> 4 ) & 0x3;
	int aIdx = ( channelMask >> 6 ) & 0x3;
	
	FragColor = vec4( col[ rIdx ], col[ gIdx ], col[ bIdx ], col[ aIdx ] );
}
