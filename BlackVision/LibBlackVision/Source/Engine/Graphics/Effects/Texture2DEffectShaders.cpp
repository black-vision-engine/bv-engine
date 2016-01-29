#include "stdafx.h"

#include "Texture2DEffectShaders.h"


namespace bv {

namespace {

// *********************************
//
std::string ps_no_alpha = " #version 400 \n \
                                \n \
                                layout (location = 0) out vec4 FragColor; \n \
                                \n \
                                in vec2 uvCoord; \n \
                                \n \
                                uniform sampler2D Texture; \n \
                                \n \
                                void main() \n \
                                {\n \
	                                vec4 col = texture( Texture, uvCoord );\n \
	                                FragColor = col.rgba;\n \
                                }\n \
                            ";

// *********************************
//
std::string ps_alpha    = " #version 400 \n \
                                \n \
                                layout (location = 0) out vec4 FragColor; \n \
                                \n \
                                in vec2 uvCoord; \n \
                                \n \
                                uniform float alpha; \n \
                                uniform sampler2D Texture; \n \
                                \n \
                                void main() \n \
                                {\n \
	                                vec4 col = texture( Texture, uvCoord );\n \
                                    //if( col.a * alpha < 0.9 ) \n \
                                    //    discard; \n \
	                                FragColor = col.rgba * alpha;\n \
                                }\n \
                            ";

// *********************************
//
std::string ps_mask_no_alpha = " #version 400 \n \
                                \n \
                                subroutine float blendValueGetter( vec4 col ); \n \
                                subroutine uniform blendValueGetter getBlendValue; \n \
                                \n \
                                layout (location = 0) out vec4 FragColor; \n \
                                \n \
                                in vec2 uvCoord; \n \
                                \n \
                                uniform sampler2D Texture; \n \
                                uniform sampler2D Mask; \n \
                                \n \
                                subroutine( blendValueGetter ) \n \
                                float getAlpha( vec4 col ) \n \
                                {\n \
                                    return col.a; \n \
                                }\n \
                                \n \
                                subroutine( blendValueGetter ) \n \
                                float getRed( vec4 col ) \n \
                                {\n \
                                    return col.r; \n \
                                }\n \
                                \n \
                                subroutine( blendValueGetter ) \n \
                                float getGreen( vec4 col ) \n \
                                {\n \
                                    return col.g; \n \
                                }\n \
                                \n \
                                subroutine( blendValueGetter ) \n \
                                float getBlue( vec4 col ) \n \
                                {\n \
                                    return col.b; \n \
                                }\n \
                                \n \
                                void main() \n \
                                {\n \
	                                vec4 col = texture( Texture, uvCoord );\n \
	                                vec4 mask = texture( Mask, uvCoord );\n \
                                    FragColor = getBlendValue( mask ) * col.rgba;\n \
                                }\n \
                            ";

// *********************************
//
std::string ps_mask_alpha    = " #version 400 \n \
                                \n \
                                subroutine float blendValueGetter( vec4 col ); \n \
                                subroutine uniform blendValueGetter getBlendValue; \n \
                                \n \
                                layout (location = 0) out vec4 FragColor; \n \
                                \n \
                                in vec2 uvCoord; \n \
                                \n \
                                uniform float alpha; \n \
                                uniform sampler2D Texture; \n \
                                uniform sampler2D Mask; \n \
                                \n \
                                subroutine( blendValueGetter ) \n \
                                float getAlpha( vec4 col ) \n \
                                {\n \
                                    return col.a; \n \
                                }\n \
                                \n \
                                subroutine( blendValueGetter ) \n \
                                float getRed( vec4 col ) \n \
                                {\n \
                                    return col.r; \n \
                                }\n \
                                \n \
                                subroutine( blendValueGetter ) \n \
                                float getGreen( vec4 col ) \n \
                                {\n \
                                    return col.g; \n \
                                }\n \
                                \n \
                                subroutine( blendValueGetter ) \n \
                                float getBlue( vec4 col ) \n \
                                {\n \
                                    return col.b; \n \
                                }\n \
                                \n \
                                void main() \n \
                                {\n \
	                                vec4 col = texture( Texture, uvCoord );\n \
	                                vec4 mask = texture( Mask, uvCoord );\n \
                                    //if( col.a * alpha < 0.9 ) \n \
                                    //    discard; \n \
                                    // FragColor = getBlendValue( mask ) * col.rgba * alpha;\n \
                                    FragColor = mask.a * col.rgba * alpha;\n \
                                }\n \
                            ";

// *********************************
//
std::string vs_default  = " #version 400 \n \
                                \n \
                                layout (location = 0) in vec3 vertexPosition; \n \
                                layout (location = 1) in vec2 vertexTexCoord; \n \
                                \n \
                                uniform mat4 MVP; \n \
                                \n \
                                out vec2 uvCoord; \n \
                                \n \
                                void main() \n \
                                {\n \
                                    gl_Position = MVP * vec4( vertexPosition, 1.0 );\n \
                                    uvCoord = vertexTexCoord; \n \
                                }\n \
                            ";

std::string alpha_param_name        = "alpha";
std::string texture_sampler_name    = "Texture";
std::string mask_sampler_name       = "Mask";

}


// *********************************
//
const std::string &     GetTexture2DEffectPixelShaderSource ( bool hasAlpha )
{
    if( hasAlpha )
    {
        return ps_alpha;
    }
    else
    {
        return ps_no_alpha;
    }
}

// *********************************
//
const std::string &     GetTexture2DWithMaskEffectPixelShaderSource ( bool hasAlpha )
{
    if( hasAlpha )
    {
        return ps_mask_alpha;
    }
    else
    {
        return ps_mask_no_alpha;
    }
}

// *********************************
//
const std::string &     GetTexture2DEffectVertexShaderSource()
{
    return vs_default;
}

// *********************************
//
const std::string &     GetTexture2DEffectTextureSamplerName()
{
    return texture_sampler_name;
}

// *********************************
//
const std::string &     GetTExture2DEffectMaskSamplerName   ()
{
    return mask_sampler_name;
}

// *********************************
//
const std::string &     GetTexture2DEffectAlphaParamName    ()
{
    return alpha_param_name;
}

} //bv
