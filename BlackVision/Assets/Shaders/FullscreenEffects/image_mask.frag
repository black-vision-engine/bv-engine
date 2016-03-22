#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;
uniform sampler2D       Mask0;

uniform float           blend;
uniform vec2            position;
uniform vec2            scale;
uniform int             invert;
uniform int             alphaOnly;
uniform int             maskAspect;
uniform int             softMask;
uniform int             onlyMask = 0;
uniform int             onlyObject = 0;

uniform mat4            maskTx;

uniform vec2            debugPoint0;
uniform vec2            debugPoint1;

float grayScale( vec4 color )
{
	return 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;	
}

void main()
{
    mat4 scaleMat = mat4( mat2( 1.0 / scale.x, 0, 0, 1.0 / scale.y) );

    vec2 maskUV = ( scaleMat * maskTx * vec4( uvCoord.x, uvCoord.y, 0.0, 1.0 ) ).xy - position;
    
	if( length( uvCoord - debugPoint0 ) < 0.003 )
	{
		FragColor = vec4( 1, 0, 0, 1 );
		return;
	}
	
	if( length( uvCoord - debugPoint1 ) < 0.003 )
	{
		FragColor = vec4( 1, 0, 0, 1 );
		return;
	}

	
	if( onlyMask == 1 || onlyObject == 1 )
	{	
		if( onlyMask == 1 )
		{
			FragColor += texture( Mask0, maskUV );
		}
		
		if( onlyObject == 1 )
		{
			FragColor += texture( Tex0, uvCoord );
		}
		
		return;
	}
	
    float alpha = 0.0;
	
	if( alphaOnly == 1 )
	{
		alpha = texture( Mask0, maskUV ).a;
	}
	else
	{
		alpha = grayScale( texture( Mask0, maskUV ) );
	}

    if ( invert == 1 )
    {
        alpha = 1.0 - alpha;
    }

    alpha = alpha * blend;
    
    FragColor = texture( Tex0, uvCoord ) * alpha;

}
