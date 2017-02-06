#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 color;

void main()
{
	FragColor = vec4( color.rgb * color.a, color.a );
}
