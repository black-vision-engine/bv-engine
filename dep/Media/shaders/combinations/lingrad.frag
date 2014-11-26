#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 color1;

in vec2 uvCoord;

void main()
{		
//	vec4 color1 = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 color2 = vec4(0.0, 1.0, 0.0, 1.0);
	vec2 point1 = vec2(0.0, 1.0);
	vec2 point2 = vec2(1.0, 0.0);

	vec2 v1 = uvCoord - point1;
	vec2 v2 = point2 - point1;

	float factor = dot(v1,v2) / dot(v2, v2);

	FragColor = color1 * factor + color2 * (1 - factor);
}
