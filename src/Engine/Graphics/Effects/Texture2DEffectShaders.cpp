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

std::string alpha_param_name = "alpha";
std::string texture_sampler_name = "Texture";

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
const std::string &     GetTexture2DEffectAlphaParamName    ()
{
    return alpha_param_name;
}

} //bv
