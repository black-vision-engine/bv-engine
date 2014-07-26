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

uniform float debug_alpha = 1;
uniform float debug_col_alpha = 0.3;
uniform float coveredDist;

uniform float preciseFilteringApronSize = 8.0 / 1080.0;

uniform float pixelOffset[20] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0 };


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
float nonLinearRescaleHeight( float h )
{
	if( hmPowFactor != 1.0 )
		return pow( h / hmHeightScale, hmPowFactor ) * hmHeightScale;

	return h;
}

// *****************************
//
float decodeLinearHeight( vec4 col )
{
	float scale =  hmHeightScale / ( 16.0 * hmMaxHeightValue ); //12:4 fixed point stored in R,G channels is used to encode height values

	return float( 256.0 * 255.0 * col.r + 255.0 * col.g ) * scale;
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
float getFilteredHeight( vec2 uv, float linearHeight )
{
    float dx = 1.0 / 1920.0;
    float hklen = 7.0;
    
    float wl = 1.0;
    int smpll = int( floor( hklen * windowWidth / 0.8 ) );
    int smplu = int( ceil( hklen * windowWidth / 0.8 ) );

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
        
        suml = mix( sumu, suml, smoothstep( 1.0, 0.0, fract( hklen * windowWidth ) ) );
    }
	else
    {
		suml /= wl;
	}

	return nonLinearRescaleHeight( suml );
}

// *****************************
//
float safeYMargin()
{
	return windowHeight * 20.0 / 1080.0;
}

// *****************************
//
vec4 calcHillColor( vec2 uv )
{
	return vec4( 1.0, 0.0, 0.0, 1.0 );
    if ( uv.x < coveredDist )
        return texture( CoveredDistTex, uv );
    else
    {
        vec4 c0 = texture( HillTex, uvCoord_hm );
        vec4 c1 = texture( HillTex, uvCoord_hm * 2.0 );
        vec4 c2 = texture( HillTex, uvCoord_hm * 4.0 );

        vec4 cm0 = mix( c1, c0, smoothstep( 0.45, 0.55, windowWidth ) );
        vec4 cm1 = mix( c2, cm0, smoothstep( 0.2, 0.35, windowWidth ) );

        return cm1;
        //return mix( mix( c0, c1, smoothstep( 0.4, 0.6, windowWidth ) ), c2, smoothstep( 0.3, 0.2, windowWidth ) );
    }
}

// *****************************
//
vec4 calcBackgroundColor( vec2 uv )
{
    return texture( BackgroundTex, vec2( uv.x, uv.y * 0.8 ) );
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

	float minY = nonLinearRescaleHeight( hmMinHeightValue / hmMaxHeightValue * hmHeightScale );

	return uv.y <  minY + hmOffsetY - safeYMargin();
}

// *****************************
//
bool isAboveMaxSamplableHillY( vec2 uv )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	//return uvCoord_hm.y < hmMaxHeightValue / hmMaxHeightValue * hmHeightScale + hmOffsetY;
	return uvCoord_hm.y > hmHeightScale + hmOffsetY + safeYMargin();
}

// *****************************
//
bool isInsidePreciseFilteringZone( vec2 uv, float h )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	return abs( uv.y - h - hmOffsetY ) < preciseFilteringApronSize;
}

// *****************************
//
bool isBelowPreciseFilteringZone( vec2 uv, float h )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	return uv.y < h + hmOffsetY - preciseFilteringApronSize;
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
		debugCol = vec4( 1.0, 0.0, 0.0, debug_col_alpha );
		col = vec4( texture( HillTex, uv ).rgb, debug_alpha );
	
		return finalHillCol( col, debugCol );
	}

	float hl = sampleHeightLinear( uv );
	float h = nonLinearRescaleHeight( hl );

	//CASE 3 - inside hill where only one small precission (not filtered) sample is required (less than expected height value - some thershold)
	if( isBelowPreciseFilteringZone( uv, h ) )
	{
		col = vec4( texture( HillTex, uv ).rgb, debug_alpha );
		debugCol = vec4( 0.0, 1.0, 0.0, debug_col_alpha );

		return finalHillCol( col, debugCol );
	}
	
	//CASE 4 - filtering required, so let's do it
	if( isInsidePreciseFilteringZone( uv, h ) )
	{
		float hf = getFilteredHeight( uv, hl );

		if( isBelowHillEdge( uv, hf ) )
		{
			col = vec4( texture( HillTex, uv ).rgb, debug_alpha );
			debugCol = vec4( 0.0, 0.0, 1.0, debug_col_alpha );
		}
		else
		{
			debugCol = vec4( 1.0, 1.0, 0.0, debug_col_alpha );
		}

		return finalHillCol( col, debugCol );
	}

	debugCol = vec4( 0.0, 1.0, 1.0, debug_col_alpha );

	return finalHillCol( col, debugCol );
}

// *****************************
//
void main()
{
	vec4 hillCol = hillColor( uvCoord_hm );
	vec4 bgCol = calcBackgroundColor( uvCoord_tx );

	FragColor = blend( bgCol, hillCol );

	/*
	if( hmOffsetY > uvCoord_hm.y || h + hmOffsetY < uvCoord_hm.y )
	{
		FragColor = calcBackgroundColor( uvCoord_tx );
	}
	else
	{
		FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
	}
	*/
	/*
    if( insideHillTest( h, uvCoord_hm ) )
    {
        FragColor = calcHillColor( uvCoord_tx );
    }
    else
    {
        vec4 c0 = calcHillColor( uvCoord_tx );
        vec4 c1 = calcBackgroundColor( uvCoord_tx );
        
		FragColor = c1;
        //FragColor = mix( c1, c0, smoothstep( h - 0.02, h + 0.02, uvCoord_hm.y ) );
        //FragColor = clacBackgroundColor( uvCoord_tx );
    }
	*/
}
