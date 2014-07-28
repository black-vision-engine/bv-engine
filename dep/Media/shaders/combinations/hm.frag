#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord_hm;
in vec2 uvCoord_tx;

noperspective in vec2 snapPos;

uniform sampler2D HeightMapTex;
uniform sampler2D HillTex;
uniform sampler2D CoveredDistTex;
uniform sampler2D BackgroundTex;

uniform vec2 scale;

uniform float hmOffsetYInPixels;

uniform float hmMaxHeightValue;
uniform float hmMinHeightValue;
uniform float hmHeightScale;
uniform float hmPowFactor;
uniform float hmGroundLevelHeight;

uniform vec2  hmShadowOffsetInPixels;
uniform vec4  hmShadowColor;

//constant params
uniform float safeYMarginPixels = 2.0;
uniform float aaRadius = 2.5;
uniform float kernelHLen = 12.0;

uniform float debug_alpha = 1;
uniform float debug_col_alpha = 1;
uniform float coveredDist;

//uniform float hmTexelSize = 1.0 / 3840.0;
//uniform float hmTextureSize = 3840.0;

uniform float preciseFilteringApronSize = 8.0 / 1080.0;

//uniform float pixelOffset[71] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70 };
uniform float pixelOffset[20] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0 };


// ************************************************************************************************ MATH and UTILS ************************************************************************************************

// *****************************
//
vec4 blend( vec4 bg, vec4 fg )
{
	//Calulation based calculations on the amount of filtered light:
	//sc - source color
	//sa - source alpha
	//dc - destination color
	//da - destination alpha
	//dc = sc * sa + ( 1 - sa ) * dc
	//da = 1 - (1 - sa)(1 - da) = sa + da - sa * da

	return vec4( fg.rgb * fg.a + ( 1.0 - fg.a ) * bg.rgb, fg.a + bg.a - fg.a * bg.a ); 
}

// *****************************
//
float decodeFixedPointValue( float msb, float lsb, float invScale )
{
    return ( 256.0 * 255.0 * msb + 255.0 * lsb ) * invScale; // / 16.0; FIXME: constant
}

// *****************************
//
float applyPow( float val, float scale, float powFactor )
{
	if( hmPowFactor != 1.0 )
    {
		return pow( val / scale, powFactor ) * scale;
    }

	return val;
}

// *****************************
// FIXME: constant
float decodeHeight( vec4 col )
{
	return hmHeightScale * ( decodeFixedPointValue( col.r, col.g, 1.0 / 16.0 ) - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight );
}

// *****************************
//
float decodeHeightApplyPow( vec4 col )
{
	return applyPow( decodeHeight( col ), hmHeightScale, hmPowFactor );
}

// *****************************
// FIXME: constant
float sampleHeight( vec2 uv )
{
    float x = ( uv.x * 3840.0 );
    float w = fract( x );

    //CASE w == 1 - epsilon but sampler samples next texel instead of the current one, that's why we have to force sampler to stay in the left pixel (texelsize * 0.995) at the cost of interpolation errors at the end of texel
    //but it can be noticed when the magnification ratio is more thann 2000:1 which is highly unlikely here
    float h0 = decodeHeight( texture( HeightMapTex, uv ) );
    float h1 = decodeHeight( texture( HeightMapTex, uv + vec2( 0.995 / 3840.0, 0.0 ) ) );

    return mix( h0, h1, w );
}

// *****************************
//
float sampleHeightApplyPow( vec2 uv )
{
    float h0 = decodeHeightApplyPow( texture( HeightMapTex, uv ) );
    float h1 = decodeHeightApplyPow( texture( HeightMapTex, uv + vec2( 0.995 / 3840.0, 0.0 ) ) );

    float x = ( uv.x * 3840.0 );
    float w = fract( x );

    return mix( h0, h1, w );
}


// ************************************************************************************************ BASIC QUERIES ************************************************************************************************

// *****************************
//
float pixelSizeX()
{
    return 1.0 / ( scale.x * 1920.0 );
}

// *****************************
//
float pixelSizeY()
{
    return 1.0 / ( scale.y * 1080.0 );
}

// *****************************
//
float visibleHeightFract()
{
	return ( hmMaxHeightValue - hmGroundLevelHeight ) / hmMaxHeightValue;
}

// *****************************
//
float y( vec2 uv )
{
    return uv.y - hmOffsetYInPixels / 1080.0;
}

// *****************************
//
float yNormalized( vec2 uv )
{
    //vec2 scl = 1.0 / scale;

    //vec2 start = loc * ( 1.0 - scl );
    //vec2 end = loc + scl * ( 1.0 - loc );

    return snapPos.y + ( uv.y - snapPos.y ) * scale.y;

    //return ( uv.y - ( centerY - 0.5 / scale.y ) ) * scale.y;
}

// *****************************
//
float safeYMargin()
{
	return safeYMarginPixels * pixelSizeY();
}

// *****************************
//
float aaMarginSize()
{
    return aaRadius * pixelSizeY();
}

// *****************************
//
vec2 shadowOffset()
{
	return vec2( -hmShadowOffsetInPixels.x * pixelSizeX(), -hmShadowOffsetInPixels.y * pixelSizeY() );
}

// *****************************
//
float bottomMarginSizeNormalized()
{
    return hmOffsetYInPixels / 1080.0;
}

// ************************************************************************************************ HM FILTERING ************************************************************************************************

// *****************************
//
float kernelHalfLen()
{
    return mix( 0.01, kernelHLen, 1.0 - smoothstep( 1.0, 15.0, scale.x ) );
}

// *****************************
//
float filterHeight( vec2 uv, float h )
{
    float dx = 1.0 / ( 1920.0 * scale.x ); //FIXME: magic number

    float hklen = kernelHalfLen();

    float sum = h;
    float w = 1.0;

    int i = 1;
    while ( hklen > 0.001 )
    {
        hklen -= 1.0;

        float tw = 1.0;

        if( hklen < 0.0 )
        {
            tw = hklen + 1.0;
        }

        sum += sampleHeight( uv + vec2( pixelOffset[ i ] * dx, 0.0 ) ) * tw;
        sum += sampleHeight( uv - vec2( pixelOffset[ i ] * dx, 0.0 ) ) * tw;

        ++i;
        w += 2.0 * tw;
    }

    return sum / w;
}

// *****************************
//
float filterHeightApplyPow( vec2 uv, float h )
{
	return applyPow( filterHeight( uv, h ), hmHeightScale, hmPowFactor );
}


// ************************************************************************************************ HM QUERIES ************************************************************************************************


// *****************************
//
bool isBelowOffsetY( vec2 uv )
{
	//FIXME: rescale offset appropriately
	//return y( uv ) < 0.0;

    return yNormalized( uv ) < bottomMarginSizeNormalized();
}

// *****************************
//
bool isBelowMinSamplableHillY( vec2 uv )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
    float linearMinY = ( hmMinHeightValue - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight ) * hmHeightScale;

    float minY = applyPow( linearMinY, hmHeightScale, hmPowFactor );

	return y( uv ) < minY - safeYMargin();
}

// *****************************
//
bool isAboveMaxSamplableHillY( vec2 uv )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	//return uvCoord_hm.y > nonLinearRescaleHeight( ( hmMaxHeightValue - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight ) * hmHeightScale ) + hmOffsetY + safeYMargin();
	return y( uv ) > hmHeightScale + safeYMargin();
}

// *****************************
// applyPow version: float minY = applyPow( h - preciseFilteringApronSize / visibleHeightFract(), hmHeightScale, hmPowFactor );
bool isBelowPreciseFilteringZoneBottom( vec2 uv, float h )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
    float minY = h - preciseFilteringApronSize / visibleHeightFract();

	return y( uv ) < minY;
}

// ***************************** 
// applyPow version
// float minY = applyPow( h - preciseFilteringApronSize / visibleHeightFract(), hmHeightScale, hmPowFactor );
// float maxY = applyPow( h + preciseFilteringApronSize / visibleHeightFract(), hmHeightScale, hmPowFactor );
bool isBelowPreciseFilteringZoneTop( vec2 uv, float h )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)

    float maxY = h + preciseFilteringApronSize / visibleHeightFract();

    return  y( uv ) < maxY;
}

// *****************************
//
bool isInsideFilteredSignal( vec2 uv, float h )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	//FIXME: explicit signal size
    return abs( y( uv ) - h ) < 2.0 * pixelSizeY();
}

// *****************************
//
bool isBelowHillEdge( vec2 uv, float h )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
    return y( uv ) < h;
}


// ************************************************************************************************ LOW LEVEL COLORS ************************************************************************************************


// *****************************
//
vec4 finalHillCol( vec4 col, vec4 debugCol )
{
	return blend( col, debugCol );
}

// *****************************
//
float hillAlpha( vec2 uv )
{
	//CASE 1 - outside hill
	if( isBelowOffsetY( uv ) || isAboveMaxSamplableHillY( uv ) )
	{
		return 0.0;
	}

	//CASE 2 - inside hill where no sampling is required (less than precalculated min height val)
	if( isBelowMinSamplableHillY( uv ) )
	{
		return 1.0;
	}

	float h = sampleHeight( uv );

	//CASE 3 - inside hill where only one small precission (not filtered) sample is required (less than expected height value - some thershold)
	if( isBelowPreciseFilteringZoneBottom( uv, h ) )
	{
		return 1.0;
	}

	//CASE 4 - filtering required, so let's do it
	if( isBelowPreciseFilteringZoneTop( uv, h ) )
	{
		float hf = filterHeight( uv, h );

		if( isBelowHillEdge( uv, hf ) )
		{
            return 1.0 - smoothstep( hf - aaMarginSize(), hf, y( uv ) );
		}
	}

	return 0.0;
}


// ************************************************************************************************ HIGH LEVEL COLORS ************************************************************************************************


// *****************************
//
vec4 hillColor( vec2 uv )
{
    float alpha = hillAlpha( uv );

    if( alpha > 0.0 )
    {
        return vec4( texture( HillTex, uvCoord_tx ).rgb, alpha );
    }

    return vec4( 0.0, 0.0, 0.0, 0.0 );
}

// *****************************
//
vec4 shadowHillColor( vec2 uv )
{
    float alpha = hillAlpha( uv + shadowOffset() );

    if( alpha > 0.0 )
    {
        return vec4( hmShadowColor.rgb, alpha );
    }

    return vec4( 0.0, 0.0, 0.0, 0.0 );
}

// *****************************
//
vec4 calcBackgroundColor( vec2 uv )
{
    vec4 col = texture( BackgroundTex, vec2( uv.x, uv.y ) );
    
    return vec4( 1.0 - col.rgb, col.a );
}

// *****************************
//
void main()
{
    //float x = ( uvCoord_hm.x * 3840.0 );
    //float w = fract( x );
    //float a = hillColor( uvCoord_hm ).a;
    //FragColor = vec4( w, w, w, a );

    FragColor = hillColor( uvCoord_hm );
    //FragColor = blend( calcBackgroundColor( uvCoord_tx ), blend( shadowHillColor( uvCoord_hm ), hillColor( uvCoord_hm ) ) );
	//vec4 c2 = hillColor( uvCoord_hm );

 //   vec4 c1 = shadowHillColor( uvCoord_hm );            
 //   c2 = blend( c1, c2 );

 //   vec4 c0 = calcBackgroundColor( uvCoord_tx );
 //   c2 = blend( c0, c2 );

 //   FragColor = c2;
}
