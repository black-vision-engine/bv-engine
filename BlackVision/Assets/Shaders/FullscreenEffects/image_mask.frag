#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D       Tex0;
uniform sampler2D       Mask0;

uniform float           blend;
uniform vec2            position;
uniform vec2            scale;
uniform int             invert;
uniform int             alphaOnly;
uniform int             maskAspect;
uniform int             softMask;

uniform mat4            maskTx;

void main()
{
    vec2 maskUV = ( maskTx * vec4( uvCoord.x, uvCoord.y, 0.0, 1.0 ) ).xy;
    
    float alpha = texture( Mask0, maskUV ).a;

    if ( invert == 1 )
    {
        alpha = 1.0 - alpha;
    }

    alpha = alpha * blend;
    
    vec4 color = texture( Tex0, uvCoord );
    
    if( alphaOnly == 0 )
    {
        FragColor = alpha * color;
    }
    else
    {
        FragColor = vec4( alpha * color.a );
    }
}
