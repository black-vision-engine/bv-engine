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
    
    float pixelW = 1.0 / textureSize.x;
    float pixelH = 1.0 / textureSize.y;
       
    if ( blurSize > 0 )
    {     
        for( int j = int( -blurSize ); j <= int( blurSize ); ++j )
        {
            for( int i = int( -blurSize ); i <= int( blurSize ); ++i )
            {
                vec2 pixelDelta = vec2( i * pixelW, j * pixelH );
                alpha += texture( Tex0, uvCoord + shift + pixelDelta ).a;
            }
        }
        
        alpha /= ( blurSize * 2.0 + 1.0 ) * ( blurSize * 2.0 + 1.0 );
    }
    else
    {
        alpha = texture( Tex0, uvCoord - shift ).a;
    }
    
    FragColor = ( 1.0 - col.a ) * color * alpha + col;
}
