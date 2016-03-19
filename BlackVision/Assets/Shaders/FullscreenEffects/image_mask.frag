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
uniform int             onlyMask = 1;
uniform int             onlyObject = 1;

uniform mat4            maskTx;

uniform vec2            debugPoint0;
uniform vec2            debugPoint1;

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
	
    float alpha = texture( Mask0, maskUV ).a;

    if ( invert == 1 )
    {
        alpha = 1.0 - alpha;
    }

    alpha = alpha * blend;
    
    vec4 color = texture( Tex0, uvCoord );
    
    if( alphaOnly == 0 )
    {
		FragColor = alpha * color;
    }
    else
    {
        FragColor = vec4( alpha * color.a );
    }
}
