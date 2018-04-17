#version 420

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform float hue;
uniform float saturation;
uniform float contrast;
uniform float brightness;

layout (binding = 0) uniform sampler2D Texture;


vec4 	Desaturate		(vec3 color, float Desaturation);
vec3 	RGBToHSL		(vec3 color);
float 	HueToRGB		(float f1, float f2, float hue);
vec3 	HSLToRGB		(vec3 hsl);

#define M_PI 3.1415926535897932384626433832795

#define epsilon 0.00001

float sigmoidal( float alpha, float beta, float x )
{
	if( alpha == 0 )
		alpha = epsilon;
	
	if( beta == 0 )
		return x;
	
	if( beta > 0 )
	{
		float numerator = 1 / (1 + exp(beta * (alpha - x))) - 1 / (1 + exp(beta * alpha));
        float denominator = 1 / (1 + exp(beta * (alpha - 1))) - 1 / (1 + exp(beta * alpha));
        return numerator / denominator;
	}
	else
	{
		return (
            (beta * alpha) - log(
                (
                    1 / (
                        (x / (1 + exp(beta * alpha - beta))) -
                        (x / (1 + exp(beta * alpha))) +
                        (1 / (1 + exp(beta * alpha)))
                    )
                ) - 1)
            ) / beta;
	}
}

void main()
{
	vec4 col = texture( Texture, uvCoord );

	vec3 hsl = RGBToHSL( col.rgb );
	
	// float hue_rad = hue * M_PI / 180;	
	// hsl.x = clamp( hsl.x + hue_rad, 0.f, 2 * M_PI );
	
	float hue_rad = hue / 360;	
// apply hue rotation	
	hsl.x = mod( hsl.x + hue_rad, 1.f );
// apply saturation translate
	hsl.y = clamp( hsl.y + saturation, 0.f, 1.f );
// apply brightness factor	
	hsl.z = clamp( hsl.z * brightness, 0.f, 1.f );
// apply contrast
//	hsl.y = sigmoidal( 0.5, contrast, hsl.y );
	hsl.z = sigmoidal( 0.5, contrast, hsl.z );
	hsl.z = clamp( hsl.z, 0.f, 1.f );
	
	vec3 rgb = HSLToRGB( hsl );

	FragColor.rgb = rgb;
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