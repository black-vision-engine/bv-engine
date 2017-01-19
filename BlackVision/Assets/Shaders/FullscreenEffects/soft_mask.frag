#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;

uniform float           width;
uniform float           progress;
uniform float           blankWidth;
uniform mat4			maskTx;
uniform bool 			invert;
uniform bool			alphaOnly;
uniform bool            objectOnly;
uniform bool            mirrorEnabled;

float distanceToLine( vec2 p0, vec2 pl, vec2 pldir )
{
	float d = dot(p0 - pl, pldir);
	vec2 p1 = pl + pldir * d;
	return distance( p0, p1 );
}

bool orientation( vec2 p, vec2 u, vec2 p0 )
{
	return (((p0.x - p.x) * u.y) / u.x) - p.y > 0;
}

float linearGradientAlpha( vec2 uv )
{
	vec2 p = ( vec4( progress, 0.0, 0.0, 1.0) * maskTx ).xy;
	vec2 v = ( vec4( 0.0, 1.0, 0.0, 0.0) * maskTx ).xy;

	float d = distanceToLine( uv, p, v );
	
	float alpha = 1.0;
	
	if ( orientation( p, v, uv ) || mirrorEnabled )
	{
		alpha = min( max( 0.0, ( width + blankWidth - d ) / width ) , 1.0 );
	}
	
	
	if( invert )
		alpha = 1.0 - alpha;
		
	return alpha;
}

void main()
{   
    FragColor = texture( Tex0, uvCoord ) * linearGradientAlpha( uvCoord );
}
