#version 400

layout (location = 0) out vec4 FragColor;

in vec2 GTexCord;

uniform sampler2D Tex0;

void main()
{
	vec4 texColor0 = texture( Tex0, GTexCord);

	FragColor = texColor0.bgra;
}
