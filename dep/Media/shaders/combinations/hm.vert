#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

uniform sampler2D HeightMapTex;

uniform mat4 MVP;

uniform vec2 scale;

uniform float totalDistanceInMeters;
uniform float curDistanceInMeters;
uniform float startDistInMeters;
uniform float endDistInMeters;

uniform float hmOffsetYInPixels;

uniform float hmMaxHeightValue;
uniform float hmMinHeightValue;
uniform float hmHeightScale;
uniform float hmGroundLevelHeight;

uniform float kernelHLenVS = 20.0;

uniform float pixelOffsetVS[30] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0, 21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0 };

out vec2 uvCoord_hm;
out vec2 uvCoord_background;
out vec2 uvCoord_coveredDist;
out vec2 uvCoord_hill;

noperspective out vec2 snapPos;

// *****************************
//
vec2 normalizedPixelCoord( vec2 posInPixels )
{
    return posInPixels * vec2( 1.0 / 1920.0, 1.0 / 1080.0 );
}

// *****************************
//
float decodeFixedPointValue( float msb, float lsb, float invScale )
{
    return ( 256.0 * 255.0 * msb + 255.0 * lsb ) * invScale; // / 4096.0;
}

// *****************************
//
float decodeHeight( vec4 col )
{
	return hmHeightScale * ( decodeFixedPointValue( col.r, col.g, 1.0 / 16.0 ) - hmGroundLevelHeight ) / ( hmMaxHeightValue - hmGroundLevelHeight );
}

// *****************************
// FIXME: constant
float sampleHeight( vec2 uv )
{
    float x = ( uv.x * 3840.0 );
    float w = fract( x );

    //CASE w == 1 - epsilon but sampler samples next texel instead of the current one
    float h0 = decodeHeight( texture( HeightMapTex, uv ) );
    float h1 = decodeHeight( texture( HeightMapTex, uv + vec2( 0.995 / 3840.0, 0.0 ) ) );

    return mix( h0, h1, w );
}

// *****************************
//FIXME: to make it look smoother at the cost of not being snapped exactly to the hill's edge
float kernelHalfLen()
{
    return mix( kernelHLenVS * 0.25, kernelHLenVS, 1.0 - smoothstep( 1.0, 15.0, scale.x ) );
}

// *****************************
//
float filterHeight( vec2 uv, float h )
{
    float dx = 1.0 / 3840.0; //FIXME: magic number

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
        
        sum += sampleHeight( uv + vec2( pixelOffsetVS[ i ] * dx, 0.0 ) ) * tw;
        sum += sampleHeight( uv - vec2( pixelOffsetVS[ i ] * dx, 0.0 ) ) * tw;

        ++i;
        w += 2.0 * tw;
    }

    return sum / w;
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
vec2 calcHillUV( vec2 uv )
{
    return uv / pow( scale, vec2( 0.05, 0.05 ) );
}

// *****************************
//
vec2 calcCoveredDistUV( vec2 uv )
{
    return uv / pow( scale, vec2( 0.055, 0.055 ) );
}

// *****************************
//
vec2 calcBackgroundUV( vec2 uv )
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
    uvCoord_background = calcBackgroundUV( vertexTexCoord );
    uvCoord_coveredDist = calcCoveredDistUV( vertexTexCoord );
    uvCoord_hill = calcHillUV( vertexTexCoord );

    gl_Position = MVP * vec4( vertexPosition, 1.0 );
}
