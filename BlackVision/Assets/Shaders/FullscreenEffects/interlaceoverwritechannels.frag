#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D 	Tex0;	// Current texture
layout (binding = 1) uniform sampler2D 	Tex1; 	// Previous texture

uniform int 		useInterlace;
uniform int			startEven;
uniform int 		height;

uniform int			channelMask;

uniform int			overwriteAlpha;
uniform float 		alpha;

// *********************************
//
int 	Y			()
{
	return (int)( (1.0 - uvCoord.y) * height);
}

// *********************************
//
vec4 InterlaceEven	()
{
	return Y() & 0x1 ? texture( Tex1 ) : texture( Tex0 );
}

// *********************************
//
vec4 InterlaceOdd	()
{
	return Y() & 0x1 ? texture( Tex0 ) : texture( Tex1 );
}

// *********************************
//
vec4 	ReadInterlaced		()
{
	return startEven ? InterlaceEven() : InterlaceOdd();
}

// *********************************
//
vec4 	ReadNonInterlaced	()
{
	return texture( Tex0 );
}

// *********************************
//
vec4	ReadPixel 			()
{
	return useInterlace ? ReadInterlaced() : ReadNonInterlaced();
}

// *********************************
//
vec4 	ApplyMaskChannels 		( vec4 col )
{
	// Apply channel mask logic
	int rIdx = ( channelMask >> 0 ) & 0x3;
	int gIdx = ( channelMask >> 2 ) & 0x3;
	int bIdx = ( channelMask >> 4 ) & 0x3;
	int aIdx = ( channelMask >> 6 ) & 0x3;

	return vec4( col[ rIdx ], col[ gIdx ], col[ bIdx ], col[ aIdx ] );
}

// *********************************
//
vec4 	ApplyOverwriteAlpha 	( vec4 col )
{
	return overwriteAlpha ? vec4( col.r, col.g, col.b, alpha ) : col;
}

// *********************************
//
void main()
{
	FragColor = ApplyOverwriteAlpha( ApplyMaskChannels( ReadPixel() ) );
}
