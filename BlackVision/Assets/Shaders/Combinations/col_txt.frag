#version 450

layout (location = 0) out vec4 FragColor;

in vec2             uvCoord;

uniform sampler2D   Tex0;

uniform vec4        color;
uniform vec4        outlineColor;
uniform float       alpha;

void main()
{
    float col1 = texture( Tex0, uvCoord ).b;
    float col2 = texture( Tex0, uvCoord ).g;
    int l = textureQueryLevels(Tex0);
        
    if( l == 1)
        FragColor = vec4(1.0, 1.0, 1.0, 1.0) * alpha;
    else
        FragColor = vec4(1.0, 0.0, 0.0, 1.0) * alpha;
    
    //FragColor = textureLod( Tex0, uvCoord, 0 );
	//FragColor = alpha * ( color * col1 + outlineColor * ( col2 * ( 1.0 - col1 ) ) );
}
