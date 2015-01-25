#version 400

layout ( location = 0 ) out vec4 FragColor;

in vec2 uvAlphaCoord;

uniform sampler2D AlphaTex0;

uniform vec4 color;

void main()
{		
    vec4 alphaMask = texture( AlphaTex0, uvAlphaCoord );
    FragColor = color * alphaMask.a;
}
