#version 400

layout (location = 0) out vec4 FragColor;

in vec2 texCord;

uniform float alpha;
uniform mat4 txMat0;
uniform mat4 txMat1;

uniform sampler2D Tex0;
uniform sampler2D Tex1;

void main()
{
    vec2 tex0Coord = ( txMat0 * vec4( texCord, 0.0, 0.0 ) ).xy;
    vec2 tex1Coord = ( txMat1 * vec4( texCord, 0.0, 0.0 ) ).xy;

	vec4 texColor0 = texture( Tex0, tex0Coord );
	vec4 texColor1 = texture( Tex1, tex1Coord );

	FragColor = mix( texColor0, texColor1, alpha ).bgra; // vec4(1.0, 1.0,1.0,1.0);

}
