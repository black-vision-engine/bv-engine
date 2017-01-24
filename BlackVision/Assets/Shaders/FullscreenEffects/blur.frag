#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;

uniform float			blurSize;
uniform vec2			textureSize;
uniform bool            vertical;
uniform bool            normalize = true;
uniform int             blurKernelType = 0;

float blurSizeCeil = ceil( blurSize );
float blurSizeFloor = floor( blurSize );
float subPixelWeight = blurSize - blurSizeFloor;

// float d = ( blurSizeCeil / 4 );

// float gauss[ 5 ] = float[]( 0.22508352 , 0.11098164 , 0.01330373 , 0.00038771 , 0.0 );
// //float gauss[ 5 ] = float[]( 1.0 , 0.75 , 0.5 , 0.25 , 0.0 );

// float evaluateWeight( float dist )
// {
    // switch( blurKernelType )
    // {
        // case 0: // Box filter
            // return 1.0;
            
        // case 1: // Triangle filter
			// dist = abs( dist );
            // return max( 0, ( ( blurSize ) - dist ) ) / ( blurSize );

        // case 2: // Gaussian filter
			// dist = abs( dist );
            // float l = floor( dist / d );
            // float r = ceil( dist / d );

            // float t = ( dist / d ) - l;

            // return mix( gauss[ int( l ) ], gauss[ int( r ) ], t );

        // default:
            // return 1.0;      
    // }
// }

void pass0()
{
    int t = int( blurSizeFloor );

    float pixelW = 1.0 / textureSize.x;
    
    float w1 = subPixelWeight;
    
    float weight = 2 * w1;
    
    vec4 sum = texture( Tex0, uvCoord - vec2( pixelW * blurSizeCeil, 0 ) ) * weight;
      
	sum += texture( Tex0, uvCoord );
	  
	weight += 1;
	  
	vec2 pixelDelta = vec2( -pixelW * 0.5, 0 );
	  
    for( int i = 1; i <= t / 2; ++i )
    {   
        pixelDelta.x += 2 * pixelW;
        sum += texture( Tex0, uvCoord + pixelDelta );
		sum += texture( Tex0, uvCoord - pixelDelta );
        weight += 2;
    }
	
	if( t % 2 != 0 )
	{
		sum += texture( Tex0, uvCoord + t * pixelDelta );
		sum += texture( Tex0, uvCoord - t * pixelDelta );
	}

    sum += texture( Tex0, uvCoord + vec2( pixelW * blurSizeCeil, 0 ) ) * w1;
    
    if( normalize )
    {
        sum /= weight;
    }
    
    FragColor = sum;  
}

void pass1()
{
    int t = int( blurSizeFloor );

    float pixelH = 1.0 / textureSize.y;
    
    float w1 = subPixelWeight;
    
    float weight = 2 * w1;
    
    vec4 sum = texture( Tex0, uvCoord - vec2( pixelH * blurSizeCeil, 0 ) ) * weight;
      
	vec2 pixelDelta = vec2( 0, -pixelH * 0.5 );
	  
	sum += texture( Tex0, uvCoord );
	weight += 1;
	  
    for( int i = 1; i <= t / 2; ++i )
    {   
		pixelDelta.y += 2 * pixelH;
        sum += texture( Tex0, uvCoord + pixelDelta );
		sum += texture( Tex0, uvCoord - pixelDelta );
        weight += 2;
    }

	if( t % 2 != 0 )
	{
		sum += texture( Tex0, uvCoord + t * pixelDelta );
		sum += texture( Tex0, uvCoord - t * pixelDelta );
	}

	
    sum += texture( Tex0, uvCoord + vec2( pixelH * blurSizeCeil, 0 ) ) * w1;
    
    if( normalize )
    {
        sum /= weight;
    }
    
    FragColor = sum;
}

void main()
{
	if ( vertical )
		pass0();
	else
		pass1();
}

