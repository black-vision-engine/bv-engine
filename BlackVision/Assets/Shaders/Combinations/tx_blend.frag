#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;
in vec2 uvCoordBlend;

uniform sampler2D Tex0;
uniform sampler2D BlendTex0;

uniform float		alpha;
uniform float		alpha2;
uniform int			blendingMode;

// blendingMode constant
#define Normal 		0
#define Lighten		1
#define Darken		2
#define Multiply	3
#define Average		4
#define Add			5
#define Substract	6
#define Difference	7
#define Negation	8
#define Exclusion	9
#define Screen		10
#define Overlay		11
#define SoftLight	12
#define HardLight	13
#define ColorDodge	14
#define ColorBurn	15
#define LinearDodge	16
#define LinearBurn	17
#define LinearLight	18
#define VividLight	19
#define PinLight	20
#define HardMix		21
#define Reflect		22
#define Glow		23
#define Phoenix		24
#define Hue			25
#define Saturation	26
#define Color		27
#define Luminosity	28

vec4	Blend			( vec4 color1, vec4 color2, int mode );
vec4	BlendNormal		( vec4 color1, vec4 color2 );
vec4	BlendLighten	( vec4 color1, vec4 color2 );
vec4	BlendDarken		( vec4 color1, vec4 color2 );
vec4	BlendMultiply	( vec4 color1, vec4 color2 );
vec4	BlendAverage	( vec4 color1, vec4 color2 );
vec4	BlendAdd		( vec4 color1, vec4 color2 );
vec4	BlendSubstract	( vec4 color1, vec4 color2 );
vec4	BlendDifference	( vec4 color1, vec4 color2 );
vec4	BlendNegation	( vec4 color1, vec4 color2 );
vec4	BlendExclusion	( vec4 color1, vec4 color2 );
vec4	BlendScreen		( vec4 color1, vec4 color2 );
vec4	BlendOverlay	( vec4 color1, vec4 color2 );
vec4	BlendSoftLight	( vec4 color1, vec4 color2 );
vec4	BlendHardLight	( vec4 color1, vec4 color2 );
vec4	BlendColorDodge	( vec4 color1, vec4 color2 );
vec4	BlendColorBurn	( vec4 color1, vec4 color2 );
vec4	BlendLinearDodge( vec4 color1, vec4 color2 );
vec4	BlendLinearBurn	( vec4 color1, vec4 color2 );
vec4	BlendLinearLight( vec4 color1, vec4 color2 );
vec4	BlendVividLight	( vec4 color1, vec4 color2 );
vec4	BlendPinLight	( vec4 color1, vec4 color2 );
vec4	BlendHardMix	( vec4 color1, vec4 color2 );
vec4	BlendReflect	( vec4 color1, vec4 color2 );
vec4	BlendGlow		( vec4 color1, vec4 color2 );
vec4	BlendPhoenix	( vec4 color1, vec4 color2 );
vec4	BlendHue		( vec4 color1, vec4 color2 );
vec4	BlendSaturation	( vec4 color1, vec4 color2 );
vec4	BlendColor		( vec4 color1, vec4 color2 );
vec4	BlendLuminosity	( vec4 color1, vec4 color2 );


void main()
{
	vec4 color1 = texture( Tex0, uvCoord );
	vec4 color2	= texture( BlendTex0, uvCoordBlend );
	
	color1 *= alpha;
	color2 *= alpha2;
	// Premultiply
	color1.rgb = color1.rgb * color1.a;
	color2.rgb = color2.rgb * color2.a;
	
	FragColor = Blend( color1, color2, blendingMode );
}

// ===================================== //
vec4	Blend		( vec4 color1, vec4 color2, int mode )
{
	if( mode == Normal )
		return BlendNormal( color1, color2 );
	else if( mode == Lighten )
		return BlendLighten( color1, color2 );
	else if( mode == Darken )
		return BlendDarken( color1, color2 );
	else if( mode == Multiply )
		return BlendMultiply( color1, color2 );
	else if( mode == Average )
		return BlendAverage( color1, color2 );
	else if( mode == Add )
		return BlendAdd( color1, color2 );
	else if( mode == Substract )
		return BlendSubstract( color1, color2 );
	else if( mode == Difference )
		return BlendDifference( color1, color2 );
	else if( mode == Negation )
		return BlendNegation( color1, color2 );
	else if( mode == Exclusion )
		return BlendExclusion( color1, color2 );
	else if( mode == Screen )
		return BlendScreen( color1, color2 );
	else if( mode == Overlay )
		return BlendOverlay( color1, color2 );
	else if( mode == SoftLight )
		return BlendSoftLight( color1, color2 );
	else if( mode == HardLight )
		return BlendHardLight( color1, color2 );
	else if( mode == ColorDodge )
		return BlendColorDodge( color1, color2 );
	else if( mode == ColorBurn )
		return BlendColorBurn( color1, color2 );
	else if( mode == LinearDodge )
		return BlendLinearDodge( color1, color2 );
	else if( mode == LinearBurn )
		return BlendLinearBurn( color1, color2 );
	else if( mode == LinearLight )
		return BlendLinearLight( color1, color2 );	
	else if( mode == VividLight )
		return BlendVividLight( color1, color2 );
	else if( mode == PinLight )
		return BlendPinLight( color1, color2 );
	
	
	return vec4( 0.0, 0.0, 0.0, 0.0 );
}

// ===================================== //
vec4	BlendNormal		( vec4 color1, vec4 color2 )
{
	return color1 + color2;
}

// ===================================== //
vec4	BlendLighten	( vec4 color1, vec4 color2 )
{
	return max( color1, color2 );
}

// ===================================== //
vec4	BlendDarken		( vec4 color1, vec4 color2 )
{
	return min( color1, color2 );
}

// ===================================== //
vec4	BlendMultiply	( vec4 color1, vec4 color2 )
{
	return color1 * color2;
}

// ===================================== //
vec4	BlendAverage	( vec4 color1, vec4 color2 )
{
	return ( color1 + color2 ) / 2;
}

// ===================================== //
vec4	BlendAdd		( vec4 color1, vec4 color2 )
{
	// OpenGL clamps automatically
	return color1 + color2;
}

// ===================================== //
vec4	BlendSubstract	( vec4 color1, vec4 color2 )
{
	// OpenGL clamps automatically
	return color1 - color2;
}

// ===================================== //
vec4	BlendDifference	( vec4 color1, vec4 color2 )
{
	return abs( color1 - color2 );
}

// ===================================== //
vec4	BlendNegation	( vec4 color1, vec4 color2 )
{
	return vec4(1.0) - abs( vec4( 1.0f ) - ( color1 + color2 ) );
}

// ===================================== //
vec4	BlendExclusion	( vec4 color1, vec4 color2 )
{
	return color1 + color2 - 2.0f * color1 * color2;
}

// ===================================== //
vec4	BlendScreen		( vec4 color1, vec4 color2 )
{
	return vec4( 1 ) - ( vec4( 1 ) - color1 ) * ( vec4( 1 ) - color2 );
}

// ===================================== //
vec4	BlendOverlay	( vec4 color1, vec4 color2 )
{
	//color1 < 0.5 ? (2.0 * color1 * color2) : (1.0 - 2.0 * (1.0 - color1) * (1.0 - color2))
	
	vec4 result;
	vec4 col12 = 2.0 * color1 * color2;
	
	color1.r < 0.5 ? result.r = col12.r : result.r = 2.0 * ( color1.r + color2.r ) - 1.0 - col12.r;
	color1.g < 0.5 ? result.g = col12.g : result.g = 2.0 * ( color1.g + color2.g ) - 1.0 - col12.g;
	color1.b < 0.5 ? result.b = col12.b : result.b = 2.0 * ( color1.b + color2.b ) - 1.0 - col12.b;
	color1.a < 0.5 ? result.a = col12.a : result.a = 2.0 * ( color1.a + color2.a ) - 1.0 - col12.a;
	
	return result;
}

// ===================================== //
vec4	BlendSoftLight	( vec4 color1, vec4 color2 )
{
	//(color2 < 0.5) ?
	//(2.0 * color1 * color2 + color1 * color1 * (1.0 - 2.0 * color2))
	//: (sqrt(color1) * (2.0 * color2 - 1.0) + 2.0 * color1 * (1.0 - color2))
	
	vec4 result;
	vec4 col1Mul2 = 2.0 * color1 * color2;
	
	result.r = color2.r < 0.5	?  col1Mul2.r + pow( color1.r, 2.0 ) - col1Mul2.r * color1.r
								: sqrt( color1.r ) * ( 2.0 * color2.r - 1.0 ) + 2.0 * color1.r - col1Mul2.r;

	result.g = color2.g < 0.5	? col1Mul2.g + pow( color1.g, 2.0 ) - col1Mul2.g * color1.g
								: sqrt( color1.g ) * ( 2.0 * color2.g - 1.0 ) + 2.0 * color1.g - col1Mul2.g;

	result.b = color2.b < 0.5	? col1Mul2.b + pow( color1.b, 2.0 ) - col1Mul2.b * color1.b
								: sqrt( color1.b ) * ( 2.0 * color2.b - 1.0 ) + 2.0 * color1.b - col1Mul2.b;

	result.a = color2.a < 0.5	? col1Mul2.a + pow( color1.a, 2.0 ) - col1Mul2.a * color1.a
								: result.a = sqrt( color1.a ) * ( 2.0 * color2.a - 1.0 ) + 2.0 * color1.a - col1Mul2.a;
					
	return result;
}

// ===================================== //
vec4	BlendHardLight	( vec4 color1, vec4 color2 )
{
	return BlendOverlay( color2, color1 );
}

// ===================================== //
vec4	BlendColorDodge	( vec4 color1, vec4 color2 )
{
	//((color2 == 1.0) ? color2 : min(color1 / (1.0 - color2), 1.0))
	
	vec4 result;
	
	// OpenGL clamps automatically
	result.r = color2.r == 1.0 ? color2.r : color1.r / (1.0 - color2.r );
	result.g = color2.g == 1.0 ? color2.g : color1.g / (1.0 - color2.g );
	result.b = color2.b == 1.0 ? color2.b : color1.b / (1.0 - color2.b );
	result.a = color2.a == 1.0 ? color2.a : color1.a / (1.0 - color2.a );
	
	return result;	
}

// ===================================== //
vec4	BlendColorBurn	( vec4 color1, vec4 color2 )
{
	//(color2 == 0.0) ? color2 : max((1.0 - ((1.0 - color1) / color2)), 0.0)
	
	vec4 result;
	
	// OpenGL clamps automatically
	result.r = color2.r == 0.0 ? color2.r : 1.0 - ( 1.0 - color1.r ) / color2.r;
	result.g = color2.g == 0.0 ? color2.g : 1.0 - ( 1.0 - color1.g ) / color2.g;
	result.b = color2.b == 0.0 ? color2.b : 1.0 - ( 1.0 - color1.b ) / color2.b;
	result.a = color2.a == 0.0 ? color2.a : 1.0 - ( 1.0 - color1.a ) / color2.a;
	
	return result;	
}

// ===================================== //
vec4	BlendLinearDodge( vec4 color1, vec4 color2 )
{
	return BlendAdd( color1, color2 );
}

// ===================================== //
vec4	BlendLinearBurn( vec4 color1, vec4 color2 )
{
	return BlendSubstract( color1, color2 );
}

// ===================================== //
vec4	BlendLinearLight( vec4 color1, vec4 color2 )
{
	//(color2 < 0.5 ? BlendLinearBurn(color1, (2.0 * color2)) : BlendLinearDodge(color1, (2.0 * (color2 - 0.5))))
	
	vec4 linearBurn = BlendLinearBurn( color1, vec4( 2.0 ) * color2 );
	vec4 linearDodge = BlendLinearDodge( color1, vec4( 2.0 ) * color2 - vec4( 1.0 ) );
	
	bvec4 threshold = lessThan( color2, vec4( 0.5 ) );
	
	return vec4( threshold ) * linearBurn + vec4( not( threshold ) ) * linearDodge;
}

// ===================================== //
vec4	BlendVividLight	( vec4 color1, vec4 color2 )
{
	// (color2 < 0.5) ? BlendColorBurnf(color1, (2.0 * color2)) : BlendColorDodgef(color1, (2.0 * (color2 - 0.5)))
	
	vec4 colorBurn = BlendColorBurn( color1, vec4( 2.0 ) * color2 );
	vec4 colorDodge = BlendColorDodge( color1, vec4( 2.0 ) * color2 - vec4( 1.0 ) );
	
	bvec4 threshold = lessThan( color2, vec4( 0.5 ) );
	
	return vec4( threshold ) * colorBurn + vec4( not( threshold ) ) * colorDodge;
}

// ===================================== //
vec4	BlendPinLight	( vec4 color1, vec4 color2 )
{
	//(color2 < 0.5) ? BlendDarkenf(color1, (2.0 * color2)) : BlendLightenf(color1, (2.0 *(color2 - 0.5)))

	vec4 darken = BlendDarken( color1, vec4( 2.0 ) * color2 );
	vec4 lighten = BlendLighten( color1, vec4( 2.0 ) * color2 - vec4( 1.0 ) );
	
	bvec4 threshold = lessThan( color2, vec4( 0.5 ) );
	
	return vec4( threshold ) * darken + vec4( not( threshold ) ) * lighten;
}

