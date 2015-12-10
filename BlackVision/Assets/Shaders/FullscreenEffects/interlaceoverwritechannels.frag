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

int 	Y			()
{
	return (int)( (1.0 - uvCoord.y) * height);
}

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

void main()
{
	//Read interlaced data
	vec4 intres;

	if( startEven )
	{
		intres = InterlaceEven();
	}
	else
	{
		intres = InterlaceOdd();
	}

	// Apply channel mask logic
	int rIdx = ( channelMask >> 0 ) & 0x3;
	int gIdx = ( channelMask >> 2 ) & 0x3;
	int bIdx = ( channelMask >> 4 ) & 0x3;
	int aIdx = ( channelMask >> 6 ) & 0x3;
	
	vec4 res( intres[ rIdx ], intres[ gIdx ], intres[ bIdx ], intres[ aIdx ] );

	// Apply overwrite alpha logic
	if( overwriteAlpha )
	{
		res.a = alpha;
	}

	FragColor = res;
}

void main()
{	

	FragColor = res;
}
