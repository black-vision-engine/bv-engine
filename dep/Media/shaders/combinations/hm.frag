#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord_hm;
in vec2 uvCoord_tx;

uniform sampler2D HeightMapTex;
uniform sampler2D HillTex;
uniform sampler2D CoveredDistTex;
uniform sampler2D BackgroundTex;

uniform float windowWidth;

uniform float hmOffsetY;

uniform float hmMaxHeightValue;
uniform float hmMinHeightValue;
uniform float hmHeightScale;

uniform float alpha;
uniform float coveredDist;

uniform float pixelOffset[20] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0 };

// *****************************
//
float decodeHeight( vec4 col )
{
	//float combined_scale = hmHeightScale / ( 4.0 * hmMaxHeightValue );
	float combined_scale = hmHeightScale / ( 4.0 * hmMaxHeightValue );

    return float( 256.0 * 255.0 * col.r + 255.0 * col.g ) / 16.0 / hmMaxHeightValue * hmHeightScale;
}

// *****************************
//
float getHeight( vec2 uv )
{
    float dx = 1.0 / 1920.0;
    float hklen = 7.0;
    
    float wl = 1.0;
    int smpll = int( floor( hklen * windowWidth / 0.8 ) );
    int smplu = int( ceil( hklen * windowWidth / 0.8 ) );

    float suml = decodeHeight( texture( HeightMapTex, uv ) );

    return suml;
    int i = 1;
    for(; i < smpll; ++i )
    {
        suml += decodeHeight( texture( HeightMapTex, uv + vec2( pixelOffset[ i ] * dx, 0.0 ) ) );
        suml += decodeHeight( texture( HeightMapTex, uv - vec2( pixelOffset[ i ] * dx, 0.0 ) ) );
        wl += 2.0;
    }

    if( smpll < smplu )
    {
        float wu = wl;
        float sumu = suml;
        
        sumu += decodeHeight( texture( HeightMapTex, uv + vec2( pixelOffset[ i ] * dx, 0.0 ) ) );
        sumu += decodeHeight( texture( HeightMapTex, uv - vec2( pixelOffset[ i ] * dx, 0.0 ) ) );
        
        wu += 2.0;
        
        suml /= ( wl );
        sumu /= ( wu );
        
        return mix( sumu, suml, smoothstep( 1.0, 0.0, fract( hklen * windowWidth ) ) );
    }
    
    return suml / ( wl * 1009.1532 );
}

// *****************************
//
bool insideHillTest( float h, vec2 uv )
{    
    float off_y = uv.y - hmOffsetY;//OFFSET

    if( off_y < 0.0 || uv.x < 0.3 )
        return false;

    if( h > off_y * 3.5f ) //SCALE_VAL, OFFSET_VAL
        return true;

    return false;
}

// *****************************
//
vec4 yebaneSuoneczko( vec2 pos )
{
    float dx = ( pos.x - 0.8 ) * 1.0;
    float dy = ( pos.y - 0.8 ) * 1080.0 / 1920.0;
    
    float dist = sqrt( dx * dx + dy * dy );

    vec4 inner_col = vec4( 1.0, 1.0, 0.0, 1.0 );
    vec4 outer_col = vec4( 0.6, 0.2, 1.0, 0.5 );

    return mix( inner_col, outer_col, smoothstep( 0.07, 0.12, dist ) );
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
bool isBelowMinHillY( vec2 uv )
{
	//FIXME: rescale hmOffset appropriately
	return uv.y < hmMinHeightValue / hmMaxHeightValue * hmHeightScale + hmOffsetY;
}

// *****************************
//
bool isBelowMaxHillY( vec2 uv )
{
	//FIXME: rescale hmOffset appropriately
	//FIXME: add offset for precise calculations (one pixel or so, so that at the top edge there are no artifacts)
	//return uvCoord_hm.y < hmMaxHeightValue / hmMaxHeightValue * hmHeightScale + hmOffsetY;
	return uvCoord_hm.y < hmHeightScale + hmOffsetY;
}

// *****************************
//
void main()
{
    float h = getHeight( uvCoord_hm );

	if( isBelowOffsetY( uvCoord_hm ) )
	{
		FragColor = calcBackgroundColor( uvCoord_tx );
	}
	else if( isBelowMinHillY( uvCoord_hm ) )
	{
		FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
	}
	else if( isBelowMaxHillY( uvCoord_hm ) )
	{
		FragColor = vec4( 0.0, 0.0, 1.0, 1.0 );		
	}
	else
	{
		FragColor = calcBackgroundColor( uvCoord_tx );
	}

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
