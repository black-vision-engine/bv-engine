#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoord;

uniform sampler2D HeightMapTex;

uniform mat4 MVP;

uniform vec2 scale;
uniform vec2 snapToPixel;

uniform float kernelHLen = 12.0;

out vec2 uvCoord_hm;
out vec2 uvCoord_tx;

// *****************************
//
vec2 normalizedPixelCoord( vec2 posInPixels )
{
    return posInPixels * vec2( 1.0 / 1920.0, 1.0 / 1080.0 );
}

// *****************************
//
vec2 calcHeightMapUV( vec2 uv )
{
    //vec2 scl = 1.0 / scale;

    //vec2 start = loc * ( 1.0 - scl );
    //vec2 end = loc + scl * ( 1.0 - loc );

    vec2 pos = normalizedPixelCoord( snapToPixel );

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
float calcNormalizedSnapY( vec2 uv )
{
    float hf = filterHeight( uv, sampleHeight( uv ) );

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
