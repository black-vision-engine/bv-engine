#version 400

layout (location = 0) out vec4 FragColor;

in vec2 texCord;

uniform sampler2D Tex0;

void main()
{
	vec4 texColor = texture(Tex0, texCord);

	FragColor = texColor.bgra; // vec4(1.0, 1.0,1.0,1.0);
}
