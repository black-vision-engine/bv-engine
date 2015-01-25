#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 color1;
uniform vec4 color2;

uniform vec2 point1;
uniform vec2 point2;

in vec2 uvCoord;
in vec2 gradCoord;

uniform sampler2D   AtlasTex0;

void main()
{		
	vec2 v1 = gradCoord - point1;
	vec2 v2 = point2 - point1;

	float factor = dot(v1,v2) / dot(v2, v2);

	FragColor = color1 * factor + color2 * (1 - factor);

	vec4 colText = texture( AtlasTex0, uvCoord );
	FragColor = FragColor * colText;
//	FragColor = colText;
}
