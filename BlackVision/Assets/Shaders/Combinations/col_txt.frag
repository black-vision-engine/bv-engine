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

// linearly colorize characters
uniform vec4        rcc_beginColor;
uniform vec4        rcc_endColor;

vec4 linearColorInterpolation( vec4 begin, vec4 end, int i, int total )
{   
    float t = float( i + 1 ) / float( total );
    
    return begin * ( 1.0 - t ) + end * t;
}


void main()
{
    float col1 = texture( AtlasTex0, uvCoord ).b;
    float col2 = texture( AtlasTex0, uvCoord ).g;
    
    vec4 c = color;
    
    if( int( time ) % 2 == 0 )
    {
        c = linearColorInterpolation( rcc_beginColor, rcc_endColor, cc_num, cc_num_total );
    }
    
	FragColor = alpha * ( c * col1 + outlineColor * ( col2 * ( 1.0 - col1 ) ) );
}


