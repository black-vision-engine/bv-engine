#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       BluredTex0;
uniform sampler2D       Tex0;

uniform vec4			color;
uniform vec2			textureSize;
uniform bool			inner;
uniform bool			outer;
uniform float       	glowStrength;

void main()
{
	vec4 col = texture( Tex0, uvCoord );
    
    float alpha  = texture( BluredTex0, uvCoord ).a;
    
	if( outer )
		FragColor = ( 1.0 - col.a ) * color * alpha * glowStrength + col;
    
    if( inner )
        FragColor = FragColor + ( 1.0 - alpha ) * color * glowStrength * col.a + alpha * col;
		
	if( !outer && !inner )
		discard;
}
