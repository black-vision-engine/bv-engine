#version 400

layout (location = 0) out vec4 FragColor;

in vec2 texCord;

uniform sampler2D Tex0;

void main()
{
	vec4 texColor0 = texture(Tex0, texCord);
	vec4 texColor1 = texture(Tex0, texCord * 2.0);

	FragColor = mix( texColor0, texColor1, 0.5 ).bgra; // vec4(1.0, 1.0,1.0,1.0);
}
