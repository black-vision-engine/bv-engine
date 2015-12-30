#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;

uniform vec4			color;
uniform vec2			shift;
uniform float			blurSize;
uniform vec2			textureSize;

void main()
{
	vec4 col = texture( Tex0, uvCoord );

    float alpha = 0.0;
    
    for( int j = int( -blurSize ); j <= int( blurSize ); ++j )
    {
        for( int i = int( -blurSize ); i <= int( blurSize ); ++i )
        {
            alpha += texture( Tex0, uvCoord + vec2( ( float( i ) / textureSize.x ), ( float( j ) / textureSize.y ) ) ).a;
        }
    }
    alpha /= ( blurSize * 2.0 + 1.0 ) * ( blurSize * 2.0 + 1.0 );
    FragColor = ( 1.0 - col.a ) * color * alpha + col;
	//FragColor = color * alpha * col.a;
}