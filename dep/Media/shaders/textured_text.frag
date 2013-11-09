#version 400

layout (location = 0) out vec4 FragColor;

in vec2 			VAtlasTexCord;
in vec2 			VTex0Cord;

uniform sampler2D 	Tex0;
uniform sampler2D 	AtlasTex;

void main()
{
	vec4 atlasTexColor = texture( AtlasTex, VAtlasTexCord );
	vec4 texColor = texture( Tex0, VTex0Cord );
	
	FragColor = atlasTexColor * texColor;

}
