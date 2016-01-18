#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;

uniform float			blurSize;
uniform vec2			textureSize;
uniform int             vertical;

void pass0()
{
    float blurSizeFloor = floor( blurSize );
    float blurSizeCeil = ceil( blurSize );
    float subPixelWeight = blurSize - blurSizeFloor;
    int t = int( blurSizeFloor );

    float pixelW = 1.0 / textureSize.x;
    
    vec4 sum = texture( Tex0, uvCoord - vec2( pixelW * blurSizeCeil, 0 ) ) * subPixelWeight;
    
    float weight = 1.0;
    for( int i = -t; i <= t; ++i )
    {
        // if( ( int( blurSize ) - abs( i ) )  < 2 )
        // {
            // weight = 1.0 / ( blurSize * 2.0 + 1.0 );
        // }
        // else
        // {
            // weight = 1.0;
        // }
        
        vec2 pixelDelta = vec2( i * pixelW, 0 );
        sum += texture( Tex0, uvCoord + pixelDelta ) * weight;
    }

    sum += texture( Tex0, uvCoord + vec2( pixelW * blurSizeCeil, 0 ) ) * subPixelWeight;
    
    sum /= ( blurSize * 2.0 + 1.0 );
    FragColor = sum;  
}

void pass1()
{
    float blurSizeFloor = floor( blurSize );
    float blurSizeCeil = ceil( blurSize );
    float subPixelWeight = blurSize - blurSizeFloor;
    int t = int( blurSizeFloor );

    float pixelH = 1.0 / textureSize.y;

    vec4 sum = texture( Tex0, uvCoord - vec2( 0, pixelH * blurSizeCeil ) ) * subPixelWeight;
    
    float weight = 1.0;
    
    for( int i = -t; i <= t; ++i )
    {
        // if( ( int( blurSize ) - abs( i ) )  < 2 )
        // {
            // weight = 1.0 / ( blurSize * 2.0 + 1.0 );
        // }
        // else
        // {
            // weight = 1.0;
        // }
    
        vec2 pixelDelta = vec2( 0, i * pixelH );
        sum += texture( Tex0, uvCoord + pixelDelta ) * weight;
    }

    sum += texture( Tex0, uvCoord + vec2( 0, pixelH * blurSizeCeil ) ) * subPixelWeight;
    
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

