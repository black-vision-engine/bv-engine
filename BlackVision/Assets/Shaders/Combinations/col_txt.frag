#version 400

layout (location = 0) out vec4 FragColor;

in vec2             uvCoord;

uniform sampler2D   AtlasTex0;

uniform vec4        color;
uniform vec4        outlineColor;
uniform float       alpha;
uniform int         cc_num;
uniform int         cc_num_total;

uniform float       time;

uniform int         colTextEffectId;

// linearly colorize characters
uniform vec4        rcc_beginColor;
uniform vec4        rcc_endColor;

vec4 gradientColor( vec4 begin, vec4 end, int i, int total )  // color gradient
{   
    float t = float( i + 1 ) / float( total );
    
    return begin * ( 1.0 - t ) + end * t;
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
    int x1 = pseudoRandonInt( i, total );
    
    float t = float( x1 ) / float( total );
    
    return linearInterpolation( begin, end, t );
}

vec4 pseudoRandonColorCube( vec4 begin, vec4 end, int i, int total )  // Linear congruential generator
{   
    int seed = pseudoRandonInt( i, total );
    
    float x = linearInterpolation( begin.x, end.x, float( seed ) / float( total ) );

    seed = pseudoRandonInt( seed, total );
    float y = linearInterpolation( begin.y, end.y, float( seed ) / float( total ) );

    seed = pseudoRandonInt( seed, total );
    float z = linearInterpolation( begin.z, end.z, float( seed ) / float( total ) );

    seed = pseudoRandonInt( seed, total );
    float w = linearInterpolation( begin.w, end.w, float( seed ) / float( total ) );
    
    return vec4( x, y, z, w );
}

vec4 pseudoRandonColorAffine( vec4 begin, vec4 end, int i, int total )  // Linear congruential generator
{   
    int x1 = ( ( total / 2 ) * i + ( total + 2 / 3 ) ) % total;
    
    float t = float( x1 + 1 ) / float( total );
    
    return begin * ( 1.0 - t ) + end * t;
}

void main()
{
    float col1 = texture( AtlasTex0, uvCoord ).b;
    float col2 = texture( AtlasTex0, uvCoord ).g;
    
    vec4 c = color;
    vec4 oc = outlineColor;

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
    default:
        break;
    }
    
	FragColor = alpha * ( c * col1 + oc * ( col2 * ( 1.0 - col1 ) ) );
}


