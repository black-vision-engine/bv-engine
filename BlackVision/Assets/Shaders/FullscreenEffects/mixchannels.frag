#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D 	Texture;

uniform int			channelMapping;
uniform vec4		channelMask;

void main()
{
	vec4 col = texture( Texture, uvCoord );

	int rIdx = ( channelMapping >> 0 ) & 0x3;
	int gIdx = ( channelMapping >> 2 ) & 0x3;
	int bIdx = ( channelMapping >> 4 ) & 0x3;
	int aIdx = ( channelMapping >> 6 ) & 0x3;
	
    FragColor = vec4( 1.0f, 1.0f, 0.0f, 1.0f );
    // FragColor = col * channelMask;
	// FragColor = vec4( channelMask[ 0 ] * col[ rIdx ], channelMask[ 1 ] * col[ gIdx ], channelMask[ 2 ] * col[ bIdx ], channelMask[ 3 ] * col[ aIdx ] );
	// FragColor = channelMask * vec4( col[ rIdx ], col[ gIdx ], col[ bIdx ], col[ aIdx ] );
}
