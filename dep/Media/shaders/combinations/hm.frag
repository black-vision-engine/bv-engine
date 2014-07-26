#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord_hm;
in vec2 uvCoord_tx;

uniform sampler2D HeightMapTex;
uniform sampler2D HillTex;
uniform sampler2D CoveredDistTex;
uniform sampler2D BackgroundTex;

uniform float windowHeight;
uniform float windowWidth;

uniform float hmOffsetY;

uniform float hmMaxHeightValue;
uniform float hmMinHeightValue;
uniform float hmHeightScale;
uniform float hmPowFactor;
uniform float hmGroundLevelHeight;

//constant params
//uniform float 

uniform float debug_alpha = 1;
uniform float debug_col_alpha = 0;
uniform float coveredDist;

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
    return ( 256.0 * 255.0 * msb + 255.0 * lsb ) * invScale; // / 16.0;
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
//
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
//
float sampleHeight( vec2 uv )
{
	return decodeHeight( texture( HeightMapTex, uv ) );
}

// *****************************
//
float sampleHeightApplyPow( vec2 uv )
{
	return decodeHeightApplyPow( texture( HeightMapTex, uv ) );
}


// ************************************************************************************************ BASIC QUERIES ************************************************************************************************


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
    return uv.y - hmOffsetY;
}

// *****************************
//
float safeYMargin()
{
	return windowHeight * 2.0 / 1080.0;
}

// *****************************
//
float aaMarginSize()
{
    return 2.5 * windowHeight / 1080.0;
}

// ************************************************************************************************ HM FILTERING ************************************************************************************************


// *****************************
//
float filterHeight( vec2 uv, float h, float hklen )
{
    float dx = 1.0 / 1920.0;
    
	//FIXME: valid len, when windows sizes are applied
    float wl = 1.0;
    int smpll = int( floor( hklen * windowWidth + 0.5 ) );
    int smplu = int( ceil( hklen * windowWidth + 0.5 ) );

    float suml = h;

    int i = 1;
    for(; i < smpll; ++i )
    {
        suml += sampleHeight( uv + vec2( pixelOffset[ i ] * dx, 0.0 ) );
        suml += sampleHeight( uv - vec2( pixelOffset[ i ] * dx, 0.0 ) );
        wl += 2.0;
    }

    if( smpll < smplu )
    {
        float wu = wl;
        float sumu = suml;
        
        sumu += sampleHeight( uv + vec2( pixelOffset[ i ] * dx, 0.0 ) );
        sumu += sampleHeight( uv - vec2( pixelOffset[ i ] * dx, 0.0 ) );
        
        wu += 2.0;
        
        suml /= wl;
        sumu /= wu;
        
        suml = mix( sumu, suml, fract( hklen * windowWidth ) ); //FIMXE: or 1 - fract
    }
	else
    {
		suml /= wl;
	}

	return suml;
}

// *****************************
//
float filterHeightApplyPow( vec2 uv, float h, float hklen )
{
	return applyPow( filterHeight( uv, h, hklen ), hmHeightScale, hmPowFactor );
}


// ************************************************************************************************ HM QUERIES ************************************************************************************************


// *****************************
//
bool isBelowOffsetY( vec2 uv )
{
	//FIXME: rescale offset appropriately
	return y( uv ) < 0.0;
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
bool isBelowPreciseFilteringZone( vec2 uv, float h )
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
bool isInsidePreciseFilteringZone( vec2 uv, float h )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)

    float minY = h - preciseFilteringApronSize / visibleHeightFract();
    float maxY = h + preciseFilteringApronSize / visibleHeightFract();

    return  minY < y( uv ) && y( uv ) < maxY;
}

// *****************************
//
bool isInsideFilteredSignal( vec2 uv, float h )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	//FIXME: explicit signal size
    return abs( y( uv ) - h ) < 2.0 * windowHeight / 1080.0;
}

// *****************************
//
bool isBelowHillEdge( vec2 uv, float h )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
    return y( uv ) < h;
}

// *****************************
//
vec4 calcBackgroundColor( vec2 uv )
{
    return vec4( 1.0 );
    vec4 col = texture( BackgroundTex, vec2( uv.x, uv.y ) );
    
    return vec4( 1.0 - col.rgb, col.a );
}

// *****************************
//
vec4 finalHillCol( vec4 col, vec4 debugCol )
{
	return blend( col, debugCol );
}

// *****************************
//
vec4 hillColor( vec2 uv )
{
	//CASE 1 - outside hill
	if( isBelowOffsetY( uv ) || isAboveMaxSamplableHillY( uv ) )
	{
		return vec4( 0.0, 0.0, 0.0, 0.0 );
	}

	vec4 col = vec4( 0.0, 0.0, 0.0, 0.0 );
	vec4 debugCol = col;

	//CASE 2 - inside hill where no sampling is required (less than precalculated min height val)
	if( isBelowMinSamplableHillY( uv ) )
	{
		debugCol = vec4( 1.0, 0.0, 0.0, debug_col_alpha ); //RED
		col = vec4( texture( HillTex, uv ).rgb, debug_alpha );
	
		return finalHillCol( col, debugCol );
	}

	float h = sampleHeight( uv );

	//CASE 3 - inside hill where only one small precission (not filtered) sample is required (less than expected height value - some thershold)
	if( isBelowPreciseFilteringZone( uv, h ) )
	{
		col = vec4( texture( HillTex, uv ).rgb, debug_alpha );
		debugCol = vec4( 0.0, 1.0, 0.0, debug_col_alpha ); //GREEN

		return finalHillCol( col, debugCol );
	}

	//CASE 4 - filtering required, so let's do it
	if( isInsidePreciseFilteringZone( uv, h ) )
	{
		float hf = filterHeight( uv, h, 12.0 ); //FIXME: magic constant - kennel half len

		if( isBelowHillEdge( uv, hf ) )
		{
            float a = 1.0 - smoothstep( hf - aaMarginSize(), hf, y( uv ) );

			col = vec4( texture( HillTex, uv ).rgb, a );
			debugCol = vec4( 1.0, 0.0, 1.0, debug_col_alpha ); //MAGENTA
		}
		else
		{
			debugCol = vec4( 1.0, 1.0, 0.0, debug_col_alpha ); //YELLOW
		}

		return finalHillCol( col, debugCol );
	}

	debugCol = vec4( 0.0, 1.0, 1.0, debug_col_alpha ); //CYAN

	return finalHillCol( col, debugCol );
}

// *****************************
//
void main()
{
	vec4 hillCol = hillColor( uvCoord_hm );
	vec4 bgCol = calcBackgroundColor( uvCoord_tx );

	FragColor = blend( bgCol, hillCol );
}
