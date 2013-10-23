#version 400

layout (location = 0) out vec4 FragColor;

in vec2 VTexCord;

uniform sampler2D Tex0;

void main()
{
	vec4 texColor0 = texture( Tex0, VTexCord);

	FragColor = texColor0.bgra;
}
