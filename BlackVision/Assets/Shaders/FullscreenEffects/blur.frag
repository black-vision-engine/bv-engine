#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;

uniform float			blurSize;
uniform vec2			textureSize;
uniform int             vertical;
uniform int             normalize = 1;
uniform int             blurKernelType = 0;

float evaluateWeight( float dist )
{
    switch( blurKernelType )
    {
        case 0: // Box filter
            return 1.0;
            
        case 1: // Triangle filter
            return( ( blurSize + 2 ) - dist ) / ( blurSize + 2 );
            
        default:
            return 1.0;      
    }
}

void pass0()
{
    float blurSizeFloor = floor( blurSize );
    float blurSizeCeil = ceil( blurSize );
    float subPixelWeight = blurSize - blurSizeFloor;
    int t = int( blurSizeFloor );

    float pixelW = 1.0 / textureSize.x;
    
    float w1 = evaluateWeight( blurSizeCeil ) * subPixelWeight;
    
    float weight = 2 * w1;
    
    vec4 sum = texture( Tex0, uvCoord - vec2( pixelW * blurSizeCeil, 0 ) ) * weight;
      
    for( int i = -t; i <= t; ++i )
    {   
        vec2 pixelDelta = vec2( i * pixelW, 0 );
        float w = evaluateWeight( i );
        sum += texture( Tex0, uvCoord + pixelDelta ) * w;
        weight += w;
    }

    sum += texture( Tex0, uvCoord + vec2( pixelW * blurSizeCeil, 0 ) ) * w1;
    
    if( normalize != 0 )
    {
        sum /= weight;
    }
    
    FragColor = sum;  
}

void pass1()
{
    float blurSizeFloor = floor( blurSize );
    float blurSizeCeil = ceil( blurSize );
    float subPixelWeight = blurSize - blurSizeFloor;
    int t = int( blurSizeFloor );

    float pixelH = 1.0 / textureSize.y;
    
    float w1 = evaluateWeight( blurSizeCeil ) * subPixelWeight;
    
    float weight = 2 * w1;
    
    vec4 sum = texture( Tex0, uvCoord - vec2( pixelH * blurSizeCeil, 0 ) ) * weight;
      
    for( int i = -t; i <= t; ++i )
    {   
        vec2 pixelDelta = vec2( 0, i * pixelH );
        float w = evaluateWeight( i );
        sum += texture( Tex0, uvCoord + pixelDelta ) * w;
        weight += w;
    }

    sum += texture( Tex0, uvCoord + vec2( pixelH * blurSizeCeil, 0 ) ) * w1;
    
    if( normalize != 0 )
    {
        sum /= weight;
    }
    
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

