#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform sampler2D Tex1;
in vec2 texCoord;

void main()
{	
	vec4 texColor = texture( Tex1, texCoord );
	FragColor = texColor.bgra;
}
