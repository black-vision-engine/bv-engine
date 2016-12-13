#version 400

layout (location = 0) out vec4 FragColor;

in vec2             uvCoord;
in vec2             ccCenterCoord;

uniform sampler2D   AtlasTex0;

uniform vec4        color;
uniform vec4        outlineColor;
uniform vec4        shadowColor;
uniform float       alpha;
uniform int         cc_num;
uniform int         cc_num_total;

uniform bool 		glowEnabled;
uniform float       glowStrength;
uniform vec4        glowColor;

uniform int         firstTextCC;
uniform int         firstTextOutCC;
uniform int         firstTextShCC;

uniform float       time;

uniform int         colTextEffectId;

uniform float       animAlphaOffset;
uniform float       animAlpha;

// linearly colorize characters
uniform vec4        rcc_beginColor;
uniform vec4        rcc_endColor;

vec4 gradientColor( vec4 begin, vec4 end, int i, int total )  // color gradient
{   
    float t = float( i + 1 ) / float( total );
    
    return begin * ( 1.0 - t ) + end * t;
}

float rand( float co )
{
    return fract(sin(dot(vec2( co, co ) ,vec2(12.9898,78.233))) * 43758.5453);
}

int pseudoRandonInt( int seed, int total )  // Linear congruential generator
{   
    return ( 4 * seed + 1 ) % ( total + 1 );
}

float linearInterpolation( float begin, float end, float t )
{
    return begin * ( 1.0 - t ) + end * t;
}

float pseudoRandonFloat( float begin, float end, int i, int total )  // Linear congruential generator
{   
    float t = rand( float( i ) / float( total ) );
    
    return linearInterpolation( begin, end, t );
}

vec4 pseudoRandonColorCube( vec4 begin, vec4 end, int i, int total )  // Linear congruential generator
{   
    float t = rand( float( i ) / float( total ) );
    
    float x = linearInterpolation( begin.x, end.x, t );

    t = rand( float( i + 1 ) / float( total ) );
    float y = linearInterpolation( begin.y, end.y, t );

    t = rand( float( i + 2 ) / float( total ) );
    float z = linearInterpolation( begin.z, end.z, t );

    t = rand( float( i + 3 ) / float( total ) );
    float w = linearInterpolation( begin.w, end.w, t );
    
    return vec4( x, y, z, w );
}

vec4 pseudoRandonColorAffine( vec4 begin, vec4 end, int i, int total )  // Linear congruential generator
{   
    int x1 = ( ( total / 2 ) * i + ( total + 2 / 3 ) ) % total;
    
    float t = float( x1 + 1 ) / float( total );
    
    return begin * ( 1.0 - t ) + end * t;
}

float animateAlpha()
{
    float lf  = animAlphaOffset * float( cc_num_total + 1 );
    int l = int( floor( lf ) );
    
    if( l < 0 )
    {
        l = -l;
    }
    
    if( lf < 0.0 )
    {
        lf = -lf;
    }
    
    if( cc_num == l )
    {
        float so = lf - float( l );
        float realAlpha = alpha * so + animAlpha * ( 1.0 - so );
        
        return realAlpha;
    }
    else if( cc_num > l ) 
    {
        return animAlpha;
    }
    else if( cc_num < l )
    {
        return alpha;
    }
}

void main()
{
    float text = texture( AtlasTex0, uvCoord ).b;
    float outline = texture( AtlasTex0, uvCoord ).g;
	float blutedTextShadow = texture( AtlasTex0, uvCoord ).r;
	float blutedOutlineShadow = texture( AtlasTex0, uvCoord ).a;
	float bluredTextGlow = texture( AtlasTex0, uvCoord + vec2( 0.0, 0.5 )).r;
    float bluredOutlineGlow = texture( AtlasTex0, uvCoord + vec2( 0.0, 0.5 )).a;
    
    vec4 c = color;
    vec4 oc = outlineColor;
	vec4 sc = shadowColor;
    float a = alpha;

    switch( colTextEffectId )
    {
    case 1:
        c = pseudoRandonColorAffine( rcc_beginColor, rcc_endColor, cc_num, cc_num_total );
         break;
    case 2:
        c = pseudoRandonColorCube( rcc_beginColor, rcc_endColor, cc_num, cc_num_total );
        break;
    case 3:
        c = gradientColor( rcc_beginColor, rcc_endColor, cc_num, cc_num_total );
        break;
    case 4:
        a = animateAlpha();
        break;
    default:
        break;
    }

	// if( a == 0.0 )
		// discard;
    
	vec4 result;
	
	if( cc_num >= firstTextShCC && cc_num < firstTextOutCC ) 
	{
		result = a * blutedTextShadow * sc;
	}
	else if ( cc_num >= firstTextOutCC && cc_num < firstTextCC ) 
	{
		result = a * ( outline * oc * ( 1 - text ) );
		
		if( glowEnabled ) 
		{
			result = min( result +  glowStrength * outlineColor * bluredOutlineGlow, 1.0 );
		}
	}
	else 
	{
		result = a * text * c;
		
		if( glowEnabled ) 
		{
			result = min( result +  glowStrength * glowColor * bluredTextGlow, 1.0 );
		}
	}
		
	if( result.a == 0.0 )
		discard;
	FragColor = result;
}


