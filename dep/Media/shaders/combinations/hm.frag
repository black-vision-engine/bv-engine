#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D HeightMapTex;

uniform float alpha;

void main()
{
	vec4 col = texture( HeightMapTex, uvCoord );
	FragColor = col * alpha;
}
