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

    vec4 alphaMask = texture( AlphaTex0, uvAlphaCoord );
    vec4 col = color1 * factor + color2 * (1 - factor);
	FragColor = alpha * ( col * alphaMask.a );
}
