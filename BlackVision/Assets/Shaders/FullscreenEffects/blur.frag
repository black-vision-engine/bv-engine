#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;

uniform float			blurSize;
uniform vec2			textureSize;
uniform int             vertical;

void pass0()
{
    float pixelW = 1.0 / textureSize.x;

    vec4 sum = vec4( 0, 0, 0, 0 );

    for( int i = int( -blurSize ); i <= int( blurSize ); ++i )
    {
        vec2 pixelDelta = vec2( i * pixelW, 0 );
        sum += texture( Tex0, uvCoord + pixelDelta );
    }

    sum /= ( blurSize * 2.0 + 1.0 );

    FragColor = sum;
}

void pass1()
{
    float pixelH = 1.0 / textureSize.y;

    vec4 sum = vec4( 0, 0, 0, 0 );

    for( int i = int( -blurSize ); i <= int( blurSize ); ++i )
    {
        vec2 pixelDelta = vec2( 0, i * pixelH );
        sum += texture( Tex0, uvCoord + pixelDelta );
    }

    sum /= ( blurSize * 2.0 + 1.0 );

    FragColor = sum;
}

void main()
{
    switch( vertical )
    {
    case 0:
        pass0();
        break;
    case 1:
        pass1();
        break;
    default:
        FragColor = texture( Tex0, uvCoord );
        break;
    }
}
