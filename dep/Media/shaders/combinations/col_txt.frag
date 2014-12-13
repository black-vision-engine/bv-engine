#version 400

layout (location = 0) out vec4 FragColor;

in vec2             uvCoord;

uniform sampler2D   AtlasTex0;

uniform vec4        color;
uniform float       alpha;

void main()
{
    float col1 = texture( AtlasTex0, uvCoord ).b;
    float col2 = texture( AtlasTex0, uvCoord ).g;
	FragColor = vec4(1.0, 1.0, 1.0, 1.0) * ( col2 * ( 1.0 - col1 ) );
}
