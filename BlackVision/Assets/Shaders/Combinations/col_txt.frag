#version 400

layout (location = 0) out vec4 FragColor;

in vec2             uvCoord;

uniform sampler2D   AtlasTex0;

uniform vec4        color;
uniform vec4        outlineColor;
uniform float       alpha;
uniform int         cc_num;

void main()
{
    float col1 = texture( AtlasTex0, uvCoord ).b;
    float col2 = texture( AtlasTex0, uvCoord ).g;
    
    vec4 c = color;
    
    if( cc_num % 2 == 0 )
    {
        c = vec4( 1.0, 1.0, 1.0, 1.0 );
    }
    
	FragColor = alpha * ( c * col1 + outlineColor * ( col2 * ( 1.0 - col1 ) ) );
}
