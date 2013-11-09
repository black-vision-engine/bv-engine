#version 400

layout (location = 0) out vec4 FragColor;

in vec2 			VTexCord;

uniform sampler2D 	AtlasTex;
uniform vec4 		color;

void main()
{
	vec4 texColor0 = texture( AtlasTex, VTexCord );

	FragColor = texColor0 * color;

}
