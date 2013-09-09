#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4  colorOut;
uniform vec4  colorIn;

uniform float radiusOut;
uniform float radiusIn;

uniform vec2  center;

in vec2 texCoord;

void main()
{	
	FragColor = mix( colorIn, colorOut, smoothstep( radiusIn, radiusOut, length( texCoord - center ) ) );
	
}
