#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4 solidOut;
uniform vec4 solidIn;
uniform float radiusIn;
uniform float radiusOut;
uniform vec2 center;

in vec2 texCord;

void main()
{	
	float dist = length(texCord - center);
	
	float t = smoothstep(radiusIn, radiusOut, dist);
	
	FragColor = mix(solidIn, solidOut, t);
	
}
