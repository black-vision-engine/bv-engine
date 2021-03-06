#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       BluredTex0;
uniform sampler2D       Tex0;

uniform vec4			color;
uniform vec2			shift;
uniform vec2			textureSize;
uniform bool			inner;
uniform bool			outer;

void main()
{
	vec4 col = texture( Tex0, uvCoord );
    
    float alpha  = texture( BluredTex0, uvCoord + shift ).a;
    
    FragColor = ( 1.0 - col.a ) * color * alpha + col;
    
    if( inner )
    {
        FragColor = ( 1.0 - alpha ) * color * col.a + alpha * col; // inner shadow and glow
    }
	
    if ( outer )
    {
        
    }
	
     
    {
        FragColor = ( 1.0 - col.a ) * color * alpha + col + ( 1.0 - alpha ) * color * col.a + alpha * col;
    }
    
}
