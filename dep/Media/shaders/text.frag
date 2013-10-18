#version 400

layout (location = 0) out vec4 FragColor;

in vec2 			texCord;

uniform sampler2D 	Tex0;
uniform vec4 		color;

void main()
{
	vec4 texColor0 = texture( Tex0, texCord );

	FragColor = texColor0 * color;

}
