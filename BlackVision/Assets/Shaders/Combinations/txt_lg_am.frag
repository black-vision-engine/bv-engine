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
in vec2 uvAlphaCoord;

uniform sampler2D AtlasTex0;
uniform sampler2D AlphaTex0;

void main()
{		
	vec2 v1 = gradCoord - point1;
	vec2 v2 = point2 - point1;

	float factor = dot(v1,v2) / dot(v2, v2);

    vec4 color = color1 * factor + color2 * (1 - factor);
    
    float col1 = texture( AtlasTex0, uvCoord ).b;
    float col2 = texture( AtlasTex0, uvCoord ).g;
    
    vec4 alphaMask = texture( AlphaTex0, uvAlphaCoord );
    
	vec4 result = alpha * ( color * col1 + outlineColor * ( col2 * ( 1.0 - col1 ) ) ) * alphaMask.a;
	
	if( result.a == 0.0 )
		discard;
	FragColor = result;
}
