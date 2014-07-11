#version 400

layout (location = 0) out vec4 FragColor;

in vec2             uvCoord;
in vec2             uvAlphaCoord;

uniform sampler2D   AtlasTex0;
uniform sampler2D   AlphaTex0;

uniform vec4        color;
uniform float       alpha;

void main()
{
	vec4 col = texture( AtlasTex0, uvCoord );
	vec4 alphaMask = texture( AlphaTex0, uvAlphaCoord );

	FragColor = col * color * alpha * alphaMask.a;
}
