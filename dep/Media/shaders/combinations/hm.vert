#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

uniform sampler2D HeightMapTex;

uniform mat4 MVP;

uniform vec2 scale;

uniform float totalDistanceInMeters;
uniform float curDistanceInMeters;

uniform float hmOffsetYInPixels;

uniform float hmMaxHeightValue;
uniform float hmMinHeightValue;
uniform float hmHeightScale;
uniform float hmGroundLevelHeight;

uniform float kernelHLenVS = 15.0;

uniform float pixelOffsetVS[30] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0 };

out vec2 uvCoord_hm;
out vec2 uvCoord_tx;

noperspective out vec2 snapPos;

// *****************************
//
vec2 normalizedPixelCoord( vec2 posInPixels )
{
    return posInPixels * vec2( 1.0 / 1920.0, 1.0 / 1080.0 );
}

// *****************************
//
float decodeFixedPointValue( float msb, float isb, float lsb, float invScale )
{
    return ( 256.0 * 256.0 * 255.0 * msb + 256.0 * 255.0 * isb + 255.0 * lsb ) * invScale; // / 4096.0;
}

// *****************************
//
float decodeHeight( vec4 col )
{
	return hmHeightScale * ( decodeFixedPointValue( col.r, col.g, col.b, 1.0 / 4096.0 ) - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight );
}

// *****************************
//
float sampleHeight( vec2 uv )
{
	return decodeHeight( texture( HeightMapTex, uv ) );
}

// *****************************
//FIXME: to make it look smoother at the cost of not being snapped exactly to the hill's edge
float kernelHalfLen()
{
    return kernelHLenVS; // / scale.x;
}

// *****************************
//
float filterHeight( vec2 uv, float h )
{
    float dx = 1.0 / 1920.0;
    
	//FIXME: valid len, when windows sizes are applied
    float wl = 1.0;
    int smpll = int( floor( kernelHalfLen() ) );
    int smplu = int( ceil( kernelHalfLen() ) );

    float suml = h;

    int i = 1;
    for(; i < smpll; ++i )
    {
        suml += sampleHeight( uv + vec2( pixelOffsetVS[ i ] * dx, 0.0 ) );
        suml += sampleHeight( uv - vec2( pixelOffsetVS[ i ] * dx, 0.0 ) );
        wl += 2.0;
    }

    if( smpll < smplu )
    {
        float wu = wl;
        float sumu = suml;
        
        sumu += sampleHeight( uv + vec2( pixelOffsetVS[ i ] * dx, 0.0 ) );
        sumu += sampleHeight( uv - vec2( pixelOffsetVS[ i ] * dx, 0.0 ) );

        wu += 2.0;

        suml /= wl;
        sumu /= wu;
        
        suml = mix( sumu, suml, 1.0 - fract( kernelHalfLen() ) ); //FIMXE: or 1 - fract
    }
	else
    {
		suml /= wl;
	}

	return suml;
}

// *****************************
//
vec2 calcNormalizedPos()
{
    vec2 hpos = vec2( curDistanceInMeters / totalDistanceInMeters, 0 );

    return vec2( hpos.x, hmOffsetYInPixels / 1080.0 + filterHeight( hpos, sampleHeight( hpos ) ) );
}

// *****************************
//
vec2 calcHeightMapUV( vec2 uv )
{
    //vec2 scl = 1.0 / scale;

    //vec2 start = loc * ( 1.0 - scl );
    //vec2 end = loc + scl * ( 1.0 - loc );

    vec2 pos = calcNormalizedPos();

    snapPos = pos;

    return pos + ( uv - pos ) / scale;
}

// *****************************
//
vec2 calcTextureUV( vec2 uv )
{
    return uv;
}

// *****************************
//
void main()
{
    //float u = centerX + 1.0 / scale.x * ( vertexTexCoord.x - 0.5 );
    //float v = centerY + 1.0 / scale.y * ( vertexTexCoord.y - 0.5 );

    uvCoord_hm = calcHeightMapUV( vertexTexCoord );
    uvCoord_tx = calcTextureUV( vertexTexCoord );

    gl_Position = MVP * vec4( vertexPosition, 1.0 );
}
