#version 430

subroutine float blendValueGetter( vec4 col ); 
subroutine uniform blendValueGetter getBlendValue;

layout (location = 0) out vec4 FragColor; 

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D Texture;
layout (binding = 1) uniform sampler2D Mask;

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
    FragColor = getBlendValue( mask ) * col.rgba;
}
