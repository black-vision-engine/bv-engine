#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform float alpha;
layout (binding = 0) uniform sampler2D Texture;

void main()
{
    vec4 col = texture( Texture, uvCoord );
    //if( col.a * alpha < 0.9 )
    //    discard;
    FragColor = col.rgba * alpha;
}
