#version 400 

subroutine float blendValueGetter( vec4 col ); 
subroutine uniform blendValueGetter getBlendValue; 

layout (location = 0) out vec4 FragColor; 

in vec2 uvCoord; 

uniform float alpha; 
uniform sampler2D Texture; 
uniform sampler2D Mask; 

subroutine( blendValueGetter ) 
float getAlpha( vec4 col ) 
{
    return col.a; 
}

subroutine( blendValueGetter ) 
float getRed( vec4 col ) 
{
    return col.r; 
}

subroutine( blendValueGetter ) 
float getGreen( vec4 col ) 
{
    return col.g; 
}

subroutine( blendValueGetter ) 
float getBlue( vec4 col ) 
{
    return col.b; 
}

void main() 
{
    vec4 col = texture( Texture, uvCoord );
    vec4 mask = texture( Mask, uvCoord );
    //if( col.a * alpha < 0.9 ) 
    //    discard; 
    // FragColor = getBlendValue( mask ) * col.rgba * alpha;
    FragColor = mask.a * col.rgba * alpha;
}
