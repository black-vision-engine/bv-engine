#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform vec4    color1;
uniform vec4    color2;

uniform vec2    point1;
uniform vec2    point2;

uniform float 	alpha;

uniform vec4    outlineColor;

in vec2 uvCoord;
in vec2 gradCoord;

uniform sampler2D   AtlasTex0;

void main()
{		
	vec2 v1 = gradCoord - point1;
	vec2 v2 = point2 - point1;

	float factor = dot(v1,v2) / dot(v2, v2);

	// Premultiply alpha before blending
	vec4 colorPreMult1 = vec4( color1.rgb * color1.a, color1.a );
	vec4 colorPreMult2 = vec4( color2.rgb * color2.a, color2.a );
	vec4 outlineColorPreMult = vec4( outlineColor.rgb * outlineColor.a, outlineColor.a );
	
    vec4 color = colorPreMult1 * factor + colorPreMult2 * (1 - factor);
    
    float col1 = texture( AtlasTex0, uvCoord ).b;
    float col2 = texture( AtlasTex0, uvCoord ).g;
    
	vec4 result = alpha * ( color * col1 + outlineColorPreMult * ( col2 * ( 1.0 - col1 ) ) );
	
	if( result.a == 0.0 )
		discard;
	FragColor = result;
}
