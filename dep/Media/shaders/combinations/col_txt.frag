#version 400

layout (location = 0) out vec4 FragColor;

in vec2             uvCoord;

uniform sampler2D   AtlasTex0;

uniform vec4        color;
uniform float       alpha;

void main()
{
    float col = texture( AtlasTex0, uvCoord ).a;
	FragColor = color * col;
}
