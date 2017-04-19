#version 400

#define M_PI 3.1415926535897932384626433832795

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
uniform int            	polyDegree;

uniform float           aspectRatio;

float distanceToLine( vec2 p0, vec2 pl, vec2 pldir )
{
	float d = dot(p0 - pl, pldir);
	vec2 p1 = pl + pldir * d;
	return distance( p0, p1 );
}

bool orientation( vec2 p, vec2 v, vec2 p0 )
{
	return cross( vec3( v.x, v.y, 0.0 ), vec3( p0.x - p.x, p0.y - p.y, 0.0 ) ).z < 0;
}

float linearGradientAlpha1( vec2 uv, vec2 p, vec2 v )
{	
	v = normalize( v );

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


float linearGradientAlpha( vec2 uv )
{
	vec2 p = ( vec4( progress, 0.0, 0.0, 1.0) * maskTx ).xy;
	vec2 v = ( vec4( 0.0, 1.0, 0.0, 0.0) * maskTx ).xy;
		
	return linearGradientAlpha1( uv, p, v );
}

float circleGradientAlpha( vec2 uv )
{
	vec2 p = ( vec4( 0.5, 0.5, 0.0, 1.0) * maskTx ).xy;
	
	float dd = (p.x - uv.x) * (p.x - uv.x) + (p.y - uv.y) * (p.y - uv.y);
	
	float alpha = 1.0;
	
	if( dd > progress * progress || mirrorEnabled )
	{
		float d = abs(sqrt( dd ) - progress);
		
		alpha = min( max( 0.0, ( width + blankWidth - d ) / width ) , 1.0 );
	}
	
	if( invert )
		alpha = 1.0 - alpha;
		
	return alpha;
}

float polygonGradientAlpha( vec2 uv )
{
	float a = atan( uv.y - 0.5, uv.x - 0.5 );
	
	int n = int(floor( a * polyDegree / ( 2 * M_PI ) ) );

	float p0a = ( n * 2 * M_PI / polyDegree );
	float p1a = ( ( n + 1 ) * 2 * M_PI / polyDegree );
	
	vec2 p0 = vec2( progress * cos(p0a), progress * sin(p0a) ) + vec2( 0.5, 0.5 );
	vec2 p1 = vec2( progress * cos(p1a), progress * sin(p1a) ) + vec2( 0.5, 0.5 );
	
	return linearGradientAlpha1( uv, p0, p1 - p0 );
}

void main()
{   
	float alpha = 1.0;

	vec2 uvNorm = vec2( ( uvCoord.x - 0.5 ) * aspectRatio + 0.5, uvCoord.y );
	
	if(polyDegree == 1)
		alpha = linearGradientAlpha( uvNorm );
	else if(polyDegree > 1)
		alpha = polygonGradientAlpha( uvNorm );
	else
		alpha = circleGradientAlpha( uvNorm );
		
	
	if ( alphaOnly || objectOnly )
	{
		vec4 retColor = vec4( 0.0, 0.0, 0.0, 0.0 );
		if ( alphaOnly )
		{
			retColor = retColor + vec4( alpha, alpha, alpha, alpha );
		}
		
		if ( objectOnly )
		{
			retColor = retColor + texture( Tex0, uvCoord );
		}
		
		FragColor = retColor;
	}
	else
	{
		FragColor = texture( Tex0, uvCoord ) * alpha;
	}
}
