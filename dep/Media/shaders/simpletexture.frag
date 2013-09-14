#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform sampler2D Tex;

in vec2 texCord;

void main()
{
	vec4 texColor = texture(Tex, texCord);

	FragColor = texColor.bgra;
}
