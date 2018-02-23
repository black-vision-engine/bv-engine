#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D VideoInputFill;
uniform sampler2D VideoInputKey;

uniform float alpha;
uniform bool EnableKey;


void main()
{
	vec3 fillColor = texture( VideoInputFill, uvCoord ).xyz;
    float key = 1.0f;
    
    if( EnableKey )
        key = texture( VideoInputKey, uvCoord ).a;
    
	FragColor = vec4( fillColor, key ) * alpha;
}
