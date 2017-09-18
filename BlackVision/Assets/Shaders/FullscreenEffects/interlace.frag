#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D 	Tex0;
layout (binding = 1) uniform sampler2D 	Tex1;

uniform int			startEven;
uniform int 		height;

int 	Y			()
{
	return int((1.0 - uvCoord.y) * height);
}

vec4 InterlaceEven	()
{
	int y = Y();

	if( y & 0x1 )
	{
		return texture( Tex1, uvCoord );
	}
	else
	{
		return texture( Tex0, uvCoord );
	}
}

vec4 InterlaceOdd	()
{
	int y = Y();

	if( y & 0x1 )
	{
		return texture( Tex0, uvCoord );
	}
	else
	{
		return texture( Tex1, uvCoord );
	}
}

void main()
{	
	vec4 res;

	if( startEven )
	{
		res = InterlaceEven();
	}
	else
	{
		res = InterlaceOdd();
	}

	FragColor = res;
}
