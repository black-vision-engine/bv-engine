#version 400

layout (location = 0) out vec4 FragColor;

noperspective in vec2 uvCoord;
noperspective in vec2 uvAlphaCoord;

uniform sampler2D Tex0;
uniform sampler2D AlphaTex0;

uniform float alpha;

void main()
{
	vec4 col = texture( Tex0, uvCoord );
	col.rgb = col.rgb * col.a;
	
	vec4 alphaMask = texture( AlphaTex0, uvAlphaCoord );

    FragColor = col * alphaMask.a * alpha;
}
