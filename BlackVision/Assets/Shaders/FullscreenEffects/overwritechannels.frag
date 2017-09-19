#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D 	Tex0;

uniform int			channelMask;
uniform int			overwriteAlpha;
uniform float 		alpha;

void main()
{
	vec4 color = texture( Tex0, uvCoord );
	
	int rIdx = ( channelMask >> 0 ) & 0x3;
	int gIdx = ( channelMask >> 2 ) & 0x3;
	int bIdx = ( channelMask >> 4 ) & 0x3;
	int aIdx = ( channelMask >> 6 ) & 0x3;
	
	vec4 res = vec4( color[ rIdx ], color[ gIdx ], color[ bIdx ], color[ aIdx ] );

	if( overwriteAlpha != 0 )
	{
		res.a = alpha;
	}

	FragColor = res;
}
