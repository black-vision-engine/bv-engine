#version 400

layout (location = 0) out vec4 FragColor;

in vec2 VTexCord0;
in vec2 VTexCord1;
uniform sampler2D Tex0;
uniform sampler2D Tex1;
uniform float Alpha0;
uniform float Alpha1;

void main()
{
	vec4 texColor0 = texture( Tex0, VTexCord0);
	vec4 texColor1 = texture( Tex1, VTexCord1);

	FragColor = texColor0.bgra * Alpha0 + texColor1.bgra * Alpha1;
}
