#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D 	Tex0;

uniform int			channelMask;
uniform int			overwriteAlpha;
uniform float 		alpha;

void main()
{
	vec4 col = texture( Tex0, uvCoord );
	
	int rIdx = ( channelMask >> 0 ) & 0x3;
	int gIdx = ( channelMask >> 2 ) & 0x3;
	int bIdx = ( channelMask >> 4 ) & 0x3;
	int aIdx = ( channelMask >> 6 ) & 0x3;
	
	vec4 res( col[ rIdx ], col[ gIdx ], col[ bIdx ], col[ aIdx ] );

	if( overwriteAlpha )
	{
		res.a = alpha;
	}

	FragColor = res;
}
