#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D 	Tex0;

uniform int			channelMixMask;
uniform vec4 		channelMaskMask;

// *********************************
//
vec4 	ApplyMaskChannels 		( vec4 col )
{
	// Apply channel mask logic
	int rIdx = ( channelMask >> 0 ) & 0x3;
	int gIdx = ( channelMask >> 2 ) & 0x3;
	int bIdx = ( channelMask >> 4 ) & 0x3;
	int aIdx = ( channelMask >> 6 ) & 0x3;

	return vec4( col[ rIdx ], col[ gIdx ], col[ bIdx ], col[ aIdx ] ) * channelMaskMask;
}

// *********************************
//
void main()
{
	vec4 pixel = texture( Tex0, uvCoords );

	FragColor = ApplyMaskChannels( pixel );
}
