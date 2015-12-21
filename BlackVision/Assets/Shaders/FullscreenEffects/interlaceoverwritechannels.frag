#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D 	Tex0;
uniform sampler2D 	Tex1;

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
	int y = Y();

	if( y & 0x1 )
	{
		return texture( Tex1 );
	}
	else
	{
		return texture( Tex0 );
	}
}

// *********************************
//
vec4 InterlaceOdd	()
{
	int y = Y();

	if( y & 0x1 )
	{
		return texture( Tex0 );
	}
	else
	{
		return texture( Tex1 );
	}
}

// *********************************
//
vec4 	ReadInterlaced		()
{
	vec4 res;

	if( startEven > 0 )
	{
		res = InterlaceEven();
	}
	else
	{
		res = InterlaceOdd();
	}

	return res;
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
	
	vec4 res( col[ rIdx ], col[ gIdx ], col[ bIdx ], col[ aIdx ] );

	return res;
}

// *********************************
//
vec4 	ApplyOverwriteAlpha 	( vec4 col )
{
	vec4 res = col;

	// Apply overwrite alpha logic
	if( overwriteAlpha )
	{
		res.a = alpha;
	}

	return res;
}

// *********************************
//
void main()
{
	FragColor = ApplyOverwriteAlpha( ApplyMaskChannels( ReadInterlaced() ) );
}
