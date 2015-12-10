#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D 	Tex0;
uniform sampler2D 	Tex1;

uniform int			startEven;
uniform int 		height;

int 	Y			()
{
	return (int)((1.0 - uvCoord.y) * height);
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
