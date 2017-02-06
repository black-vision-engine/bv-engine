#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 color1;
uniform vec4 color2;

uniform vec2 point1;
uniform vec2 point2;

uniform float alpha;

in vec2 uvCoord;
in vec2 fadeUVCoord;


uniform vec2	FadePoint1;
uniform vec2	FadePoint2;
uniform vec2	FadePoint3;
uniform vec2	FadePoint4;

uniform float	FadeAlpha1;
uniform float	FadeAlpha2;
uniform float	FadeAlpha3;
uniform float	FadeAlpha4;

float	ComputeFadeAlpha();


void main()
{		
	vec2 v1 = uvCoord - point1;
	vec2 v2 = point2 - point1;

	// Premultiply alpha before blending
	vec4 col1 = vec4( color1.rgb * color1.a, color1.a );
	vec4 col2 = vec4( color2.rgb * color2.a, color2.a );
	
	float factor = dot(v1,v2) / dot(v2, v2);

	FragColor = ComputeFadeAlpha() * alpha * ( col1 * factor + col2 * (1 - factor) );
}


float	ComputeFadeAlpha()
{
	// linear interpolation between points FadePoint1 and FadePoint2
	vec2 v12 = FadePoint2 - FadePoint1;
	vec2 v1uv = fadeUVCoord - FadePoint1;
	
	float v12factor = dot( v12, v1uv ) / dot( v12, v12 );
	float alpha12 = v12factor * FadeAlpha1 + (1.0 - v12factor) * FadeAlpha2;
	
	// linear interpolation between points FadePoint3 and FadePoint4
	vec2 v34 = FadePoint4 - FadePoint3;
	vec2 v3uv = fadeUVCoord - FadePoint3;
	
	float v34factor = dot( v34, v3uv ) / dot( v34, v34 );
	float alpha34 = v34factor * FadeAlpha3 + ( 1.0 - v34factor ) * FadeAlpha4;
	
	// Compute new points from previously interpolated values
	vec2 newPoint5 = FadePoint1 + v12factor * v12;
	vec2 newPoint6 = FadePoint3 + v34factor * v34;
	
	// linear interpolation between points newPoint5 and newPoint6
	vec2 v56 = newPoint6 - newPoint5;
	vec2 v5uv = fadeUVCoord - newPoint5;
	
	float v56factor = dot( v56, v5uv ) / dot( v56, v56 );
	
	return v56factor * alpha12 + ( 1.0 - v56factor ) * alpha34;
}
