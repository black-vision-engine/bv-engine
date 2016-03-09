#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;
uniform sampler2D       Mask0;


void main()
{
    FragColor = texture( Tex0, uvCoord );    
}
