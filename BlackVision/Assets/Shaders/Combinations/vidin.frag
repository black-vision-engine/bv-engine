#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D VideoInput0;

uniform float alpha;


void main()
{
	vec4 col = texture( VideoInput0, uvCoord );
	FragColor = col * alpha;
}
