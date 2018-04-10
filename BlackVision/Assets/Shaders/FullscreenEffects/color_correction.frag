#version 420

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform vec4 color;

layout (binding = 0) uniform sampler2D Texture;


void main()
{
	vec4 col = texture( Texture, uvCoord );

	col += color;
	
	col = clamp( col, 0, 1 );
	
	col *= col.a;

	FragColor = col;
}
