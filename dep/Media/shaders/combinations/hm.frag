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

uniform float debug_alpha = 0.9;
uniform float debug_col_alpha = 0.7;
uniform float coveredDist;

uniform float preciseFilteringApronSize = 7.0 / 1080.0;

//uniform float pixelOffset[71] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70 };
uniform float pixelOffset[20] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0 };


// *****************************
//
float visibleHeightFract()
{
	return ( hmMaxHeightValue - hmGroundLevelHeight ) / hmMaxHeightValue;
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
float decodeLinearHeight( vec4 col )
{
	float val = float( 256.0 * 255.0 * col.r + 255.0 * col.g ) / 16.0;

	return hmHeightScale * ( val - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight );
}

// *****************************
//
float nonLinearRescaleHeight( float h )
{
	//if( hmPowFactor != 1.0 )
	//	return pow( h / hmHeightScale, hmPowFactor ) * hmHeightScale;

	return h;
}

// *****************************
//
float decodeHeight( vec4 col )
{
	float linearHeight = decodeLinearHeight( col );
	
	return nonLinearRescaleHeight( linearHeight );
}

// *****************************
//
float sampleHeightLinear( vec2 uv )
{
	return decodeLinearHeight( texture( HeightMapTex, uv ) );
}

// *****************************
//
float sampleHeight( vec2 uv )
{
	return decodeHeight( texture( HeightMapTex, uv ) );
}

// *****************************
//
float getFilteredLinearHeight( vec2 uv, float linearHeight )
{
    float dx = 1.0 / 1920.0;
    float hklen = 12.0; //FIXME: hardcoded kernel len
    
	//FIXME: valid len, when windows sizes are applied
    float wl = 1.0;
    int smpll = int( floor( hklen * windowWidth + 0.5 ) );
    int smplu = int( ceil( hklen * windowWidth + 0.5 ) );

    float suml = linearHeight;

    int i = 1;
    for(; i < smpll; ++i )
    {
        suml += sampleHeightLinear( uv + vec2( pixelOffset[ i ] * dx, 0.0 ) );
        suml += sampleHeightLinear( uv - vec2( pixelOffset[ i ] * dx, 0.0 ) );
        wl += 2.0;
    }

    if( smpll < smplu )
    {
        float wu = wl;
        float sumu = suml;
        
        sumu += sampleHeightLinear( uv + vec2( pixelOffset[ i ] * dx, 0.0 ) );
        sumu += sampleHeightLinear( uv - vec2( pixelOffset[ i ] * dx, 0.0 ) );
        
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
float getFilteredHeight( vec2 uv, float linearHeight )
{
	return nonLinearRescaleHeight( getFilteredLinearHeight( uv, linearHeight ) );
}

// *****************************
//
float safeYMargin()
{
	return windowHeight * 2.0 / 1080.0;
}

// *****************************
//
vec4 calcBackgroundColor( vec2 uv )
{
    return texture( BackgroundTex, vec2( uv.x, uv.y ) );
}

// *****************************
//
bool isBelowOffsetY( vec2 uv )
{
	//FIXME: rescale offset appropriately
	return uv.y < hmOffsetY;
}

// *****************************
//
bool isBelowMinSamplableHillY( vec2 uv )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)

	float minY = nonLinearRescaleHeight( ( hmMinHeightValue - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight ) * hmHeightScale );

	return uv.y <  minY + hmOffsetY - safeYMargin();
}

// *****************************
//
bool isAboveMaxSamplableHillY( vec2 uv )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	//return uvCoord_hm.y > nonLinearRescaleHeight( ( hmMaxHeightValue - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight ) * hmHeightScale ) + hmOffsetY + safeYMargin();
	return uvCoord_hm.y > hmHeightScale + hmOffsetY + safeYMargin();
}

// *****************************
//
bool isBelowPreciseFilteringZoneLinearHeight( vec2 uv, float hl )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	return uv.y < nonLinearRescaleHeight( hl - preciseFilteringApronSize / visibleHeightFract() ) + hmOffsetY;
}

// *****************************
//
bool isInsidePreciseFilteringZoneLinearHeight( vec2 uv, float hl )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	
	float y =  uv.y - hmOffsetY;

	return nonLinearRescaleHeight( hl - preciseFilteringApronSize / visibleHeightFract() ) < y && y < nonLinearRescaleHeight( hl + preciseFilteringApronSize / visibleHeightFract() );
}

// *****************************
//
bool isInsideFilteredSignal( vec2 uv, float hf )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	return abs( uv.y - hf - hmOffsetY ) < 2.0 / 1080.0;
}

// *****************************
//
bool isBelowHillEdge( vec2 uv, float hf )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	return uv.y < hf + hmOffsetY;
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

	float hl = sampleHeightLinear( uv );

	//CASE 3 - inside hill where only one small precission (not filtered) sample is required (less than expected height value - some thershold)
	if( isBelowPreciseFilteringZoneLinearHeight( uv, hl ) )
	{
		col = vec4( texture( HillTex, uv ).rgb, debug_alpha );
		debugCol = vec4( 0.0, 1.0, 0.0, debug_col_alpha ); //GREEN

		return finalHillCol( col, debugCol );
	}

	//CASE 4 - filtering required, so let's do it
	if( isInsidePreciseFilteringZoneLinearHeight( uv, hl ) )
	{
		float hf = getFilteredHeight( uv, hl );

		if( isBelowHillEdge( uv, hf ) )
		{
			col = vec4( texture( HillTex, uv ).rgb, debug_alpha );
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
