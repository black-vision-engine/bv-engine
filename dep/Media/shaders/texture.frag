#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform sampler2D Tex1;

uniform vec4 color1;
uniform vec4 color2;
uniform vec2 point1;
uniform vec2 point2;

uniform vec2 normal; // normalized vec2(point2 - point1)
uniform float invDist; // 1. / length(vec2(point2 - point1))

in vec2 texCord;

void main()
{	
	vec4 texColor = texture(Tex1, texCord);
	
	FragColor = texColor.bgra;
}
