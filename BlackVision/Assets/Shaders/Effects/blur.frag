#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D   Tex0;
uniform int         blurLenght;
uniform float       pixelWidth;
uniform float       pixelHeight;

void main()
{
    vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );
    for( int i = -blurLenght; i <= blurLenght; ++i )
        for( int j = -blurLenght; j <= blurLenght; ++j )
            sum += texture( Tex0, uvCoord + vec2( i * pixelWidth, j * pixelHeight ) );
    FragColor = sum / ( ( 2 * blurLenght + 1 ) * ( 2 * blurLenght + 1 ) );
}