#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 color1;
uniform vec4 color2;
uniform vec2 point1;
uniform vec2 point2;

uniform vec2 normal; // normalized vec2(point2 - point1)
uniform float invDist; // 1. / length(vec2(point2 - point1))

in vec2 texCord;

void main()
{	
	float t = dot((texCord - point1), normal) * invDist;

	t = clamp(t, 0., 1.);
		
	FragColor = mix(color1, color2, t);
}
