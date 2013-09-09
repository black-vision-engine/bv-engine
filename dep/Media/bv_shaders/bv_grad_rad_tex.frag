#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4  colorIn;
uniform float radiusIn;
uniform float radiusOut;
uniform vec2  center;

uniform sampler2D Tex1;

in vec2 texCoord;

void main()
{	
	FragColor = mix( colorIn, texture( Tex1, texCoord ).bgra, smoothstep( radiusIn, radiusOut, length( texCoord - center ) ) );	
}
