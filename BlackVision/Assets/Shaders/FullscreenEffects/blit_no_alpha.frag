#version 430

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D Texture;

void main()
{
    vec4 col = texture( Texture, uvCoord );
    FragColor = col.rgba;
    // FragColor = vec4(0.5, 0.5, 1.0, 1.0);
}
