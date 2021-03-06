#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;

uniform float			blurSize;
uniform vec2			textureSize;
uniform bool            vertical;
uniform bool            normalize = true;
uniform int             blurKernelType = 0;

float blurSizeCeil 	= ceil( blurSize );
float blurSizeFloor 	= floor( blurSize );
float subPixelWeight 	= blurSize - blurSizeFloor;

void pass0()
{
    int t = int( blurSizeFloor );
    float pixelW = 1.0 / textureSize.x;
    
    float weight = 0.0;
	vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );
	
	if( subPixelWeight > 0 && blurSizeCeil != blurSizeFloor )
	{
		sum += texture( Tex0, uvCoord - vec2( pixelW * blurSizeCeil, 0 ) ) * subPixelWeight * 0.5;
		sum += texture( Tex0, uvCoord + vec2( pixelW * blurSizeCeil, 0 ) ) * subPixelWeight * 0.5;
		weight += subPixelWeight;
	}
      
	sum += texture( Tex0, uvCoord ) * 0.5;
	weight += 0.5;
	  
    for( int i = 1; i <= t / 2; ++i )
    {   
		vec2 delta = vec2( pixelW * ( float( 2 * i ) - 0.5 ), 0.0 );
		
        sum += texture( Tex0, uvCoord + delta );
		sum += texture( Tex0, uvCoord - delta );
        weight += 2.0;
    }

	if( t % 2 != 0 )
	{
		sum += texture( Tex0, uvCoord + vec2( blurSizeFloor * pixelW, 0.0 ) ) * 0.5;
		sum += texture( Tex0, uvCoord - vec2( blurSizeFloor * pixelW, 0.0 ) ) * 0.5;
		weight += 1.0;
	}
    
    if( normalize )
    {
        sum = sum / weight;
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
		sum += texture( Tex0, uvCoord - vec2( 0.0, pixelH * blurSizeCeil ) ) * subPixelWeight * 0.5;
		sum += texture( Tex0, uvCoord + vec2( 0.0, pixelH * blurSizeCeil ) ) * subPixelWeight * 0.5;
		weight += subPixelWeight;
	}
      
	sum += texture( Tex0, uvCoord ) * 0.5;
	weight += 0.5;
	  
    for( int i = 1; i <= t / 2; ++i )
    {   
		vec2 delta = vec2( 0.0, pixelH * ( float( 2 * i ) - 0.5 ) );
		
        sum += texture( Tex0, uvCoord + delta );
		sum += texture( Tex0, uvCoord - delta );
        weight += 2.0;
    }

	if( t % 2 != 0 )
	{
		sum += texture( Tex0, uvCoord + vec2( 0.0, blurSizeFloor * pixelH ) ) * 0.5;
		sum += texture( Tex0, uvCoord - vec2( 0.0, blurSizeFloor * pixelH ) ) * 0.5;
		weight += 1.0;
	}
    
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

	