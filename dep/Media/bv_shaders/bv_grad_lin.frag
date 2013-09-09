#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 color1;
uniform vec4 color2;
uniform vec2 point1;

uniform vec2  normal;
uniform float invDist;

in vec2 texCoord;

void main()
{	
	FragColor = mix( color1, color2, clamp( dot( ( texCoord - point1 ), normal ) * invDist, 0.0, 1.0 ) );
}