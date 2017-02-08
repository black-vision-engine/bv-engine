#version 420

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;
in vec2 fadeUVCoord;


uniform sampler2D Tex0;
uniform float alpha;

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
	vec4 col = texture( Tex0, uvCoord );
	
	float finalAlpha = alpha * ComputeFadeAlpha();
	FragColor = col * finalAlpha;
	//FragColor = col * col.a * alpha;  //This one uses premultiplied RGB values
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


