#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D 	Tex0;	// Current texture
layout (binding = 1) uniform sampler2D 	Tex1; 	// Previous texture

uniform int 		useInterlace;
uniform int			startEven;
uniform float 		height;

uniform int			channelMask;

uniform int			overwriteAlpha;
uniform float 		alpha;

// *********************************
//
vec4 	FetchTex0	()
{
	//return vec4( 0.0, 0.0, 0.0, 1.0 );
	return texture( Tex0, uvCoord );
}

// *********************************
//
vec4 	FetchTex1	()
{
	//return vec4( 1.0, 1.0, 1.0, 1.0 );
	return texture( Tex1, uvCoord );
}

// *********************************
//
int 	Y			()
{
	return int( ( 1.0 - uvCoord.y ) * height );
}

// *********************************
//
vec4 InterlaceEven	()
{
	return ( Y() & 0x1 ) == 1 ? FetchTex1() : FetchTex0();
}

// *********************************
//
vec4 InterlaceOdd	()
{
	return ( Y() & 0x1 ) == 1 ? FetchTex0() : FetchTex1();
}

// *********************************
//
vec4 	ReadInterlaced		()
{
	return startEven > 0 ? InterlaceEven() : InterlaceOdd();
}

// *********************************
//
vec4 	ReadNonInterlaced	()
{
	return FetchTex0();
}

// *********************************
//
vec4	ReadPixel 			()
{
	return useInterlace > 0 ? ReadInterlaced() : ReadNonInterlaced();
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
	return overwriteAlpha > 0 ? vec4( col.r, col.g, col.b, alpha ) : col;
}

// *********************************
//
void main()
{
	FragColor = ApplyMaskChannels( ApplyOverwriteAlpha( ReadPixel() ) );
}
