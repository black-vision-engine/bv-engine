#version 400

layout (location = 0) out vec4 FragColor;

in vec2             uvCoord;

uniform sampler2D   AtlasTex0;

uniform vec4        color;
uniform float       alpha;

void main()
{
    vec4 col = texture( AtlasTex0, uvCoord );
    //FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );
	FragColor = col * color * alpha;
}
