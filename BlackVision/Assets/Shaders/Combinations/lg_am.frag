#version 400

layout ( location = 0 ) out vec4 FragColor;

uniform sampler2D AlphaTex0;

uniform vec4 color1;
uniform vec4 color2;

uniform vec2 point1;
uniform vec2 point2;

uniform float alpha;

in vec2 uvCoord;
in vec2 uvAlphaCoord;

void main()
{		
	vec2 v1 = uvCoord - point1;
	vec2 v2 = point2 - point1;

	float factor = dot(v1,v2) / dot(v2, v2);

	// Premultiply alpha before blending
	vec4 col1 = vec4( color1.rgb * color1.a, color1.a );
	vec4 col2 = vec4( color2.rgb * color2.a, color2.a );
	
    vec4 alphaMask = texture( AlphaTex0, uvAlphaCoord );
    vec4 col = col1 * factor + col2 * (1 - factor);
	FragColor = alpha * ( col * alphaMask.a );
}
