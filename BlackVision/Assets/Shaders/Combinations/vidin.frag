#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D VideoInputFill;
uniform sampler2D VideoInputKey;

uniform float alpha;
uniform bool enableKey;


void main()
{
	vec3 fillColor = texture( VideoInputFill, uvCoord ).xyz;
    float key = 1.0f;
    
    if( enableKey )
        key = texture( VideoInputKey, uvCoord ).x;
    
	FragColor = vec4( fillColor, key ) * alpha;
}
