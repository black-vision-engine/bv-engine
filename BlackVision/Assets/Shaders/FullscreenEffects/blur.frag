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
float subPixelWeight = 1.0 - blurSizeCeil + blurSize;

void pass0()
{
    int t = int( blurSizeFloor );

    float pixelW = 1.0 / textureSize.x;
    
    float weight = 0.0;
    
	vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );
	
	if( subPixelWeight > 0 && blurSizeCeil != blurSizeFloor )
	{
		sum += texture( Tex0, uvCoord - vec2( pixelW * blurSizeCeil, 0 ) ) * subPixelWeight;
		sum += texture( Tex0, uvCoord + vec2( pixelW * blurSizeCeil, 0 ) ) * subPixelWeight;
		weight += ( 2 * subPixelWeight );
	}
      
	sum += texture( Tex0, uvCoord );
	weight += 1;
	  
	vec2 pixelDelta = vec2( 0.0, 0.0 );//vec2( -pixelW * 0.5, 0 );
	  
    for( int i = 1; i <= t; ++i )
    {   
        pixelDelta = pixelDelta + vec2( pixelW, 0.0 );
        sum += texture( Tex0, uvCoord + pixelDelta );
		sum += texture( Tex0, uvCoord - pixelDelta );
        weight += 2;
    }
	
	// if( t % 2 != 0 )
	// {
		// sum += texture( Tex0, uvCoord + vec2( blurSizeFloor * pixelW, 0.0 ) );
		// sum += texture( Tex0, uvCoord - vec2( blurSizeFloor * pixelW, 0.0 ) );
		// weight += 2;
	// }
    
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
    
    float weight = 0.0;
    
	vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );
	
	if( subPixelWeight > 0 && blurSizeCeil != blurSizeFloor )
	{
		sum += texture( Tex0, uvCoord - vec2( 0.0, pixelH * blurSizeCeil ) ) * subPixelWeight;
		sum += texture( Tex0, uvCoord + vec2( 0.0, pixelH * blurSizeCeil ) ) * subPixelWeight;
		weight += ( 2 * subPixelWeight );
	}
      
	vec2 pixelDelta = vec2( 0.0, 0.0 );// vec2( 0, -pixelH * 0.5 );
	  
	sum += texture( Tex0, uvCoord );
	weight += 1;
	  
    for( int i = 1; i <= t; ++i )
    {   
		pixelDelta = pixelDelta + vec2( 0.0, pixelH );
        sum += texture( Tex0, uvCoord + pixelDelta );
		sum += texture( Tex0, uvCoord - pixelDelta );
        weight += 2;
    }

	// if( t % 2 != 0 )
	// {
		// sum += texture( Tex0, uvCoord + vec2( 0.0, blurSizeFloor * pixelH ) );
		// sum += texture( Tex0, uvCoord - vec2( 0.0, blurSizeFloor * pixelH ) );
		// weight += 2;
	// }
    
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

	