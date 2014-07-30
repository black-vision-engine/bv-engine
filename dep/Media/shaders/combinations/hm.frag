#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord_hm;
in vec2 uvCoord_background;
in vec2 uvCoord_coveredDist;
in vec2 uvCoord_hill;

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

uniform float totalDistanceInMeters;
uniform float curDistanceInMeters;
uniform float startDistInMeters;
uniform float endDistInMeters;

uniform float coveredDistShowFactor;

uniform vec2  hmShadowOffsetInPixels;
uniform vec4  hmShadowColor;

uniform vec4  hmOutlineInnedColor;
uniform vec4  hmOutlineEdgeColor;

//constant params
uniform float safeYMarginPixels = 2.0;
uniform float aaRadiusY = 2.5;
uniform float aaRadiusX = 2.8;
uniform float kernelHLen = 12.0;

uniform float safeDistFieldYMarginPixels = 8.5;
uniform int edgeDistHalfKernelSize = 4;
uniform float outlineThickness = 2.6;
uniform float innerThickness = 6.0;
uniform float aaOutlineRadius = 2.0;

//shitty debug params
uniform float debug_alpha = 1;
uniform float debug_col_alpha = 1;

uniform float preciseFilteringApronSize = 8.0 / 1080.0;

//ASSERT( innerThicknes + 2 * outerThickness < 2 * safeDistFieldYMarginPixels )
//ASSERT( safeDistFieldYMarginPixels < preciseFilteringApronSize * 1080 * 2 && innerThickness + 2 * outlineThickness < preciseFilteringApronSize * 1080 * 2 )

//uniform float pixelOffset[71] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70 };
uniform float pixelOffset[20] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0 };

uniform bool NVIDIA_PASS = false;


// ************************************************************************************************ MATH and UTILS ************************************************************************************************

// *****************************
//FIXME: requires fullhd textures
vec4 tex2DBilinear( sampler2D txSampler, vec2 uv )
{
    if( NVIDIA_PASS )
    {
//NVIDIA PASS
        float texelOffsetX = 0.05 / 1920.0;
        float texelOffsetY = 0.05 / 1080.0;

        float texelSizeX = 1.0 / 1920.0; //Should be 1.0 / 1920 @see sampleheight for the explanation of this scale factor
        float texelSizeY = 1.0 / 1080.0; //Should be 1.0 / 1080 @see sampleheight for the explanation of used scale factor

        vec2 textureSize = vec2( 1920.0, 1080.0 );

        vec4 tl = texture2D( txSampler, uv + vec2( texelOffsetX, texelOffsetY ) );
        vec4 tr = texture2D( txSampler, uv + vec2( texelSizeX, texelOffsetX ) );
        vec4 bl = texture2D( txSampler, uv + vec2( texelOffsetX, texelSizeY ) );
        vec4 br = texture2D( txSampler, uv + vec2( texelSizeX, texelSizeY ) );

        vec2 f = fract( uv * textureSize );

        vec4 ta = mix( tl, tr, f.x );
        vec4 tb = mix( bl, br, f.x );

        return mix( ta, tb, f.y );
    }
    else
    {
//ATI PASS
        float texelSizeX = 0.995 / 1920.0; //Should be 1.0 / 1920 @see sampleheight for the explanation of this scale factor
        float texelSizeY = 0.995 / 1080.0; //Should be 1.0 / 1080 @see sampleheight for the explanation of used scale factor

        vec2 textureSize = vec2( 1920.0, 1080.0 );

        vec4 tl = texture2D( txSampler, uv );
        vec4 tr = texture2D( txSampler, uv + vec2( texelSizeX, 0.0 ) );
        vec4 bl = texture2D( txSampler, uv + vec2( 0.0, texelSizeY ) );
        vec4 br = texture2D( txSampler, uv + vec2( texelSizeX, texelSizeY ) );

        vec2 f = fract( uv * textureSize );

        vec4 ta = mix( tl, tr, f.x );
        vec4 tb = mix( bl, br, f.x );

        return mix( ta, tb, f.y );
    }
}

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

    if( NVIDIA_PASS )
    {
//NVIDIA PASS
        float h0 = decodeHeight( texture( HeightMapTex, uv + vec2( 0.005 / 3840.0, 0.0 )) );
        float h1 = decodeHeight( texture( HeightMapTex, uv + vec2( 1.0 / 3840.0, 0.0 ) ) );

        return mix( h0, h1, w );
    }
    else
    {
//ATI PASS
        float h0 = decodeHeight( texture( HeightMapTex, uv ) );
        float h1 = decodeHeight( texture( HeightMapTex, uv + vec2( 0.995 / 3840.0, 0.0 ) ) );

        return mix( h0, h1, w );
    }
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
float calcHmOffsetYInPixels()
{
    return hmOffsetYInPixels;
}

// *****************************
//
float y( vec2 uv )
{
    return uv.y - calcHmOffsetYInPixels() / 1080.0;
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
float safeDistFieldYMargin()
{
    return safeDistFieldYMarginPixels * pixelSizeY();
}

// *****************************
//
float aaMarginSizeX()
{
    return aaRadiusX * pixelSizeX();
}

// *****************************
//
float aaMarginSizeY()
{
    return aaRadiusY * pixelSizeY();
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
    return calcHmOffsetYInPixels() / 1080.0;
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
//applyPow version: minY = applyPow( minY, hmHeightScale, hmPowFactor )
bool isBelowMinSamplableEdgeMargin( vec2 uv )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
    float minY = ( hmMinHeightValue - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight ) * hmHeightScale;
	return y( uv ) < minY - safeDistFieldYMargin();
}

// *****************************
//
bool isAboveMaxSamplableEdgeMargin( vec2 uv )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	//return uvCoord_hm.y > nonLinearRescaleHeight( ( hmMaxHeightValue - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight ) * hmHeightScale ) + hmOffsetY + safeYMargin();
	return y( uv ) > hmHeightScale + safeDistFieldYMargin();
}

// *****************************
//
bool isInsideSignificantDistScalarField( vec2 uv, float h )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	//FIXME: explicit signal size
    return abs( y( uv ) - h ) < 2.0 * safeDistFieldYMargin();
}

// *****************************
//applyPow version: minY = applyPow( minY, hmHeightScale, hmPowFactor )
bool isBelowMinSamplableHillY( vec2 uv )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
    float minY = ( hmMinHeightValue - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight ) * hmHeightScale;
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
float distFromHillScalarField( vec2 uv )
{
	//CASE 1 - outside hill
	if( isBelowMinSamplableEdgeMargin( uv - shadowOffset() * 0.5 ) || isAboveMaxSamplableEdgeMargin( uv ) )
	{
		return innerThickness + outlineThickness + 1.0; //FIXME: some predefined MAX val
	}

    //should sample at all
    float h = sampleHeight( uv );

    if( !isInsideSignificantDistScalarField( uv, h ) )
    {
        return innerThickness + outlineThickness + 1.0; //FIXME: some predefined MAX val
    }

    float hf = filterHeight( uv, h );

    if( !isInsideSignificantDistScalarField( uv, hf ) )
    {
        return innerThickness + outlineThickness + 1.0; //FIXME: some predefined MAX val
    }

    //if close enough - do not sample
    float y     = y( uv );
    float yscl  = 1.0 / pixelSizeY();
    float dx = pixelSizeX();
    vec2 pt     = vec2( 0.0, yscl * ( y - hf ) );
    
    /*
    if( abs( y - hf ) * yscl < innerThickness * 0.5 ) //FIXME: magic number (inner pixels that do not require the whole loop
    {
        return 6.0;
    }
    */
    
    //return 4.0;

    float minD  = dot( pt, pt );

    int i = 1;
    for (; i <= edgeDistHalfKernelSize; ++i )
    {
        h  = sampleHeight( uv + vec2( pixelOffset[ i ] * dx, 0.0 ) );
        hf = filterHeight( uv + vec2( pixelOffset[ i ] * dx ), h );    
        pt = vec2( -i, yscl * ( y - hf ) );
        minD = min( minD, dot( pt, pt ) );

        h  = sampleHeight( uv - vec2( pixelOffset[ i ] * dx, 0.0 ) );
        hf = filterHeight( uv - vec2( pixelOffset[ i ] * dx ), h );    
        pt = vec2( i, yscl * ( y - hf ) );
        minD = min( minD, dot( pt, pt ) );
    }

    return sqrt( minD ) ;
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
            return 1.0 - smoothstep( hf - aaMarginSizeY(), hf, y( uv ) );
		}
	}

	return 0.0;
}

// *****************************
//
float shadowHillAlpha( vec2 uv )
{
	//CASE 1 - outside hill
	if( isBelowOffsetY( uv - shadowOffset() ) || isAboveMaxSamplableHillY( uv ) )
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
            return 1.0 - smoothstep( hf - aaMarginSizeY(), hf, y( uv ) );
		}
	}

	return 0.0;
}


// ************************************************************************************************ HIGH LEVEL COLORS ************************************************************************************************


// *****************************
//
vec4 coveredEdgeColor( vec2 uv )
{
    float dist = curDistanceInMeters / totalDistanceInMeters * coveredDistShowFactor;
    
    float startDist = startDistInMeters / totalDistanceInMeters;
    float endDist = endDistInMeters / totalDistanceInMeters;

    vec4 o = vec4( 0 );
    
    if ( uv.x > startDist && uv.x < endDist && uv.x <= dist + outlineThickness * 0.5 * pixelSizeX() + 1 * pixelSizeX() && !isBelowOffsetY( uv ) )
    {
        float d = distFromHillScalarField( uv + shadowOffset() * 0.5 );
        
        vec4 ci = hmOutlineInnedColor;
        vec4 co = hmOutlineEdgeColor;

        if( d <= innerThickness * 0.5 + outlineThickness * 0.5 )
        {
            float d0 = innerThickness * 0.5;
            float d1 = d0 + outlineThickness * 0.5;

            o = mix( ci, co, smoothstep( d0, d1, d ) );
        }
        else if( d <= innerThickness * 0.5 + outlineThickness )
        {
            float d0 = innerThickness * 0.5 + outlineThickness * 0.5;
            float d1 = d0 + outlineThickness * 0.9; //FIXME: some shitty param to make it look better when no zoom is present

            o = vec4( co.rgb, mix( 1.0, 0.0, smoothstep( d0, d1, d ) ) );
        }

        if( o.a > 0 )
        {
            d = ( uv.x - min( dist, endDist ) ) / pixelSizeX() - outlineThickness * 0.5 - 1;
            float ds = ( uv.x - startDist ) / pixelSizeX();

            if( d >= -2 )
            {
                float a = 1.0 - smoothstep( -2.0, -0.5, d );
                o = mix( o, co, a );
                o.a = a;
            }
            if( d > -4.5 && d < -2 )
            {
                float a = 1.0 - smoothstep( -4.0, -2.5, d );
                o = mix( co, o, a );
            }

            if( ds <= 3.0 )
            {
                if( ds <= 1.0 )
                {
                    float a = 0.95;
                    o = mix( o, co, a );
                    o.a = a;
                }
                else if ( ds <= 2.0 )
                {
                    o = mix( o, co, 0.5 );
                }
                else
                {
                    o = mix( o, co, 0.2 );
                }
            }
        }
    }

    return o;
}

// *****************************
//
vec4 hillColor( vec2 uv )
{
    float alpha = hillAlpha( uv );

    if( alpha > 0.0 )
    {
        float dist = curDistanceInMeters / totalDistanceInMeters * coveredDistShowFactor;

        float dmin = dist - aaMarginSizeX() * 0.5;
        float dmax = dist + aaMarginSizeX() * 0.5;

        if( uv.x < dmin )
        {
            return vec4( tex2DBilinear( CoveredDistTex, uvCoord_coveredDist ).rgb, alpha );
        }
        else if( uv.x > dmax )
        {
            return vec4( tex2DBilinear( HillTex, uvCoord_hill ).rgb, alpha );
        }
        else
        {
            vec4 c0 = vec4( tex2DBilinear( CoveredDistTex, uvCoord_coveredDist ).rgb, alpha );
            vec4 c1 = vec4( tex2DBilinear( HillTex, uvCoord_hill ).rgb, alpha );
            
            return mix( c0, c1, smoothstep( dmin, dmax, uv.x ) );
        }
    }

    return vec4( 0.0, 0.0, 0.0, 0.0 );
}

// *****************************
//
vec4 shadowHillColor( vec2 uv )
{
    float alpha = shadowHillAlpha( uv + shadowOffset() );

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
    vec4 col = tex2DBilinear( BackgroundTex, uv );
    
    return vec4( 1.0 - col.rgb, col.a );
}

// *****************************
//
void main()
{
    vec4 c2 = hillColor( uvCoord_hm );          //memoizes hf and h which is used by coveredEdgeColor
    vec4 c3 = coveredEdgeColor( uvCoord_hm );
    vec4 c1 = shadowHillColor( uvCoord_hm );
    vec4 c0 = calcBackgroundColor( uvCoord_background );

    FragColor = blend( c0, blend( blend( c1, c2 ), c3 ) );
}
