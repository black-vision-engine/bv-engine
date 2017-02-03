#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;
in vec2 uvCoordBlend;

uniform sampler2D Tex0;
uniform sampler2D BlendTex0;

uniform float		alpha;
uniform float		alpha2;
uniform int			blendMode;

// blendMode constant
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
#define None		29

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
vec4	BlendNone		( vec4 color1, vec4 color2 );


vec4 	Desaturate		(vec3 color, float Desaturation);
vec3 	RGBToHSL		(vec3 color);
float 	HueToRGB		(float f1, float f2, float hue);
vec3 	HSLToRGB		(vec3 hsl);


void main()
{
	vec4 color1 = texture( Tex0, uvCoord );
	vec4 color2	= texture( BlendTex0, uvCoordBlend );
	
	color1 *= alpha;
	color2 *= alpha2;
	// Premultiply
	color1.rgb = color1.rgb * color1.a;
	color2.rgb = color2.rgb * color2.a;
	
	FragColor = Blend( color1, color2, blendMode );
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
	else if( mode == HardMix )
		return BlendHardMix( color1, color2 );
	else if( mode == Reflect )
		return BlendReflect( color1, color2 );
	else if( mode == Glow )
		return BlendGlow( color1, color2 );
	else if( mode == Phoenix )
		return BlendPhoenix( color1, color2 );
	else if( mode == Hue )
		return BlendHue( color1, color2 );
	else if( mode == Saturation )
		return BlendSaturation( color1, color2 );
	else if( mode == Color )
		return BlendColor( color1, color2 );
	else if( mode == Luminosity )
		return BlendLuminosity( color1, color2 );
	else if( mode == None )
		return BlendNone( color1, color2 );	
	
	
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
	
	if( color1.r < 0.5 ) result.r = col12.r; else result.r = 2.0 * ( color1.r + color2.r ) - 1.0 - col12.r;
	if( color1.g < 0.5 ) result.g = col12.g; else result.g = 2.0 * ( color1.g + color2.g ) - 1.0 - col12.g;
	if( color1.b < 0.5 ) result.b = col12.b; else result.b = 2.0 * ( color1.b + color2.b ) - 1.0 - col12.b;
	if( color1.a < 0.5 ) result.a = col12.a; else result.a = 2.0 * ( color1.a + color2.a ) - 1.0 - col12.a;
	
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
	
	if( color2.r < 0.5	)
		result.r = col1Mul2.r + pow( color1.r, 2.0 ) - col1Mul2.r * color1.r;
	else
		result.r = sqrt( color1.r ) * ( 2.0 * color2.r - 1.0 ) + 2.0 * color1.r - col1Mul2.r;

	if( color2.g < 0.5	)
		result.g = col1Mul2.g + pow( color1.g, 2.0 ) - col1Mul2.g * color1.g;
	else 
		result.g = sqrt( color1.g ) * ( 2.0 * color2.g - 1.0 ) + 2.0 * color1.g - col1Mul2.g;

	if( color2.b < 0.5	)
		result.b = col1Mul2.b + pow( color1.b, 2.0 ) - col1Mul2.b * color1.b;
	else
		result.b = sqrt( color1.b ) * ( 2.0 * color2.b - 1.0 ) + 2.0 * color1.b - col1Mul2.b;

	if( color2.a < 0.5	)
		result.a = col1Mul2.a + pow( color1.a, 2.0 ) - col1Mul2.a * color1.a;
	else
		result.a = result.a = sqrt( color1.a ) * ( 2.0 * color2.a - 1.0 ) + 2.0 * color1.a - col1Mul2.a;
					
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
	if( color2.r == 1.0 ) result.r = color2.r; else result.r = color1.r / (1.0 - color2.r );
	if( color2.g == 1.0 ) result.g = color2.g; else result.g = color1.g / (1.0 - color2.g );
	if( color2.b == 1.0 ) result.b = color2.b; else result.b = color1.b / (1.0 - color2.b );
	if( color2.a == 1.0 ) result.a = color2.a; else result.a = color1.a / (1.0 - color2.a );
	
	return result;	
}

// ===================================== //
vec4	BlendColorBurn	( vec4 color1, vec4 color2 )
{
	//(color2 == 0.0) ? color2 : max((1.0 - ((1.0 - color1) / color2)), 0.0)
	
	vec4 result;
	
	// OpenGL clamps automatically
	if( color2.r == 0.0 ) result.r = color2.r; else result.r = 1.0 - ( 1.0 - color1.r ) / color2.r;
	if( color2.g == 0.0 ) result.g = color2.g; else result.g = 1.0 - ( 1.0 - color1.g ) / color2.g;
	if( color2.b == 0.0 ) result.b = color2.b; else result.b = 1.0 - ( 1.0 - color1.b ) / color2.b;
	if( color2.a == 0.0 ) result.a = color2.a; else result.a = 1.0 - ( 1.0 - color1.a ) / color2.a;
	
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

// ===================================== //
vec4	BlendHardMix	( vec4 color1, vec4 color2 )
{
	//BlendVividLightf(base, blend) < 0.5) ? 0.0 : 1.0
	vec4 color = BlendVividLight( color1, color2 );
	bvec4 threshold = lessThan( color, vec4( 0.5 ) );

	return vec4( threshold.rgb, color.a );
}

// ===================================== //
vec4	BlendReflect	( vec4 color1, vec4 color2 )
{
	//(color2 == 1.0) ? color2 : min(color1 * color1 / (1.0 - color2), 1.0)
	vec4 result;
	
	if( color2.r == 1.0 ) result.r = color2.r; else result.r = color1.r * color1.r / (1.0 - color2.r);
	if( color2.g == 1.0 ) result.g = color2.g; else result.g = color1.g * color1.g / (1.0 - color2.g);
	if( color2.b == 1.0 ) result.b = color2.b; else result.b = color1.b * color1.b / (1.0 - color2.b);
	if( color2.a == 1.0 ) result.a = color2.a; else result.a = color1.a * color1.a / (1.0 - color2.a);
	
	return result;
}

// ===================================== //
vec4	BlendGlow		( vec4 color1, vec4 color2 )
{
	return BlendReflect( color2, color1 );
}

// ===================================== //
vec4	BlendPhoenix	( vec4 color1, vec4 color2 )
{
	return min(color1, color2) - max(color1, color2) + vec4( 1.0 );
}

// ===================================== //
vec4	BlendHue		( vec4 color1, vec4 color2 )
{
	vec3 color1HSL = RGBToHSL( vec3( color1 ) );
	return vec4( HSLToRGB(vec3(RGBToHSL( vec3( color2 ) ).r, color1HSL.g, color1HSL.b)), color1.a * color2.a );
}

// ===================================== //
vec4	BlendSaturation	( vec4 color1, vec4 color2 )
{
	vec3 color1HSL = RGBToHSL( vec3( color1 ) );
	return vec4( HSLToRGB(vec3(color1HSL.r, RGBToHSL( vec3( color2 ) ).g, color1HSL.b)), color1.a * color2.a  );
}

// ===================================== //
vec4	BlendColor		( vec4 color1, vec4 color2 )
{
	vec3 color2HSL = RGBToHSL( vec3( color2 ) );
	return vec4( HSLToRGB(vec3(color2HSL.r, color2HSL.g, RGBToHSL( vec3( color1 ) ).b)), color1.a * color2.a  );
}

// ===================================== //
vec4	BlendLuminosity	( vec4 color1, vec4 color2 )
{
	vec3 color1HSL = RGBToHSL( vec3( color1 ));
	return vec4( HSLToRGB(vec3(color1HSL.r, color1HSL.g, RGBToHSL( vec3( color2 ) ).b)), color1.a * color2.a  );
}

// ===================================== //
vec4	BlendNone		( vec4 color1, vec4 color2 )
{
	return color2;
}

/*
** Copyright (c) 2012, Romain Dura romain@shazbits.com
** 
** Permission to use, copy, modify, and/or distribute this software for any 
** purpose with or without fee is hereby granted, provided that the above 
** copyright notice and this permission notice appear in all copies.
** 
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES 
** WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF 
** MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY 
** SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES 
** WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN 
** ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR 
** IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*
** Photoshop & misc math
** Blending modes, RGB/HSL/Contrast/Desaturate, levels control
**
** Romain Dura | Romz
** Blog: http://mouaif.wordpress.com
** Post: http://mouaif.wordpress.com/?p=94
*/



/*
** Desaturation
*/

vec4 Desaturate(vec3 color, float Desaturation)
{
	vec3 grayXfer = vec3(0.3, 0.59, 0.11);
	vec3 gray = vec3(dot(grayXfer, color));
	return vec4(mix(color, gray, Desaturation), 1.0);
}


/*
** Hue, saturation, luminance
*/

vec3 RGBToHSL(vec3 color)
{
	vec3 hsl; // init to 0 to avoid warnings ? (and reverse if + remove first part)
	
	float fmin = min(min(color.r, color.g), color.b);    //Min. value of RGB
	float fmax = max(max(color.r, color.g), color.b);    //Max. value of RGB
	float delta = fmax - fmin;             //Delta RGB value

	hsl.z = (fmax + fmin) / 2.0; // Luminance

	if (delta == 0.0)		//This is a gray, no chroma...
	{
		hsl.x = 0.0;	// Hue
		hsl.y = 0.0;	// Saturation
	}
	else                                    //Chromatic data...
	{
		if (hsl.z < 0.5)
			hsl.y = delta / (fmax + fmin); // Saturation
		else
			hsl.y = delta / (2.0 - fmax - fmin); // Saturation
		
		float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;
		float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;
		float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;

		if (color.r == fmax )
			hsl.x = deltaB - deltaG; // Hue
		else if (color.g == fmax)
			hsl.x = (1.0 / 3.0) + deltaR - deltaB; // Hue
		else if (color.b == fmax)
			hsl.x = (2.0 / 3.0) + deltaG - deltaR; // Hue

		if (hsl.x < 0.0)
			hsl.x += 1.0; // Hue
		else if (hsl.x > 1.0)
			hsl.x -= 1.0; // Hue
	}

	return hsl;
}

float HueToRGB(float f1, float f2, float hue)
{
	if (hue < 0.0)
		hue += 1.0;
	else if (hue > 1.0)
		hue -= 1.0;
	float res;
	if ((6.0 * hue) < 1.0)
		res = f1 + (f2 - f1) * 6.0 * hue;
	else if ((2.0 * hue) < 1.0)
		res = f2;
	else if ((3.0 * hue) < 2.0)
		res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;
	else
		res = f1;
	return res;
}

vec3 HSLToRGB(vec3 hsl)
{
	vec3 rgb;
	
	if (hsl.y == 0.0)
		rgb = vec3(hsl.z); // Luminance
	else
	{
		float f2;
		
		if (hsl.z < 0.5)
			f2 = hsl.z * (1.0 + hsl.y);
		else
			f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);
			
		float f1 = 2.0 * hsl.z - f2;
		
		rgb.r = HueToRGB(f1, f2, hsl.x + (1.0/3.0));
		rgb.g = HueToRGB(f1, f2, hsl.x);
		rgb.b= HueToRGB(f1, f2, hsl.x - (1.0/3.0));
	}
	
	return rgb;
}

