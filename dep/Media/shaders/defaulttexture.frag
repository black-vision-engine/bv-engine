#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D tex;
uniform float alpha;

void main()
{
	vec4 col = texture( tex, uvCoord );
	FragColor = col.bgra * alpha;
}
