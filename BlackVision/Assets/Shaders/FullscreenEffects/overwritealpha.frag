#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D Tex0;

uniform float alpha;

void main()
{
	vec4 col = texture( Tex0, uvCoord );
	
	FragColor = vec4( col.r, col.g, col.b, alpha );
}
