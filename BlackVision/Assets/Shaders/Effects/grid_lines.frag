#version 430

layout (location = 0) out vec4 FragColor;

in                  vec3 GPosition;

uniform vec4 color;

void main()
{
	FragColor = color;
}
