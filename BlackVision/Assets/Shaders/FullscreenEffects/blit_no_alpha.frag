#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D Texture;

void main()
{
    vec4 col = texture( Texture, uvCoord );
    FragColor = col.rgba;
}
