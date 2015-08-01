#version 400

layout (location = 0) out vec4 FragColor;

in vec2             uvCoord;
in vec2             uvAlphaCoord;

uniform sampler2D   AtlasTex0;
uniform sampler2D   AlphaTex0;

uniform vec4        color;
uniform vec4        outlineColor;
uniform float       alpha;

void main()
{
    float col1 = texture( AtlasTex0, uvCoord ).b;
    float col2 = texture( AtlasTex0, uvCoord ).g;
	vec4 alphaMask = texture( AlphaTex0, uvAlphaCoord );
	FragColor = alphaMask.a * alpha * ( color * col1 + outlineColor * ( col2 * ( 1.0 - col1 ) ) );
}
