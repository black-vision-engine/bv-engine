#pragma once

#include "Engine/Graphics/Renderers/OGLRenderer/glslprogram.h"
#include "Engine/Graphics/Shaders/ShaderParamDesc.h"

#include <glm/glm.hpp>


namespace bv
{

class Renderer;
class Shader;
class PixelShader;
class VertexShader;
class GeometryShader;
class GenericShaderParam;
class TextureSampler;

class Texture2D;
class TextureAnimatedSequence2D;

class ShaderTextureParameters;

class PdrShader
{
private:

    GLSLProgram *       m_program;

    PixelShader *       m_pixelShader;
    VertexShader *      m_vertexShader;
    GeometryShader *    m_geometryShader;

private:

    PdrShader                       ( GLSLProgram * program, PixelShader * ps, VertexShader * vs, GeometryShader * gs );

public:

    ~PdrShader                      ();

    static PdrShader *  Create      ( PixelShader * ps, VertexShader * vs, GeometryShader * gs );

public:

    void    Enable                  ( Renderer * renderer );
    void    Disable                 ( Renderer * renderer );

    //FIXME: use SamplerState to cache it at least a bit (this should be stored in current Renderer)
    //void SetSamplerState (Renderer* renderer, RendererData::SamplerState * curSamplerState ); //see comments in PdrShader.cpp

private:

    void    SetUniforms             ( Shader * shader );
    void    SetUniformParam         ( GenericShaderParam * param );

    template< ParamType paramType >
    void    SetUniformDispatcher    ( GenericShaderParam * param )
    {
        typedef ShaderParamTypeMapper< paramType >::type ValType;
        m_program->SetUniform( param->Name().c_str(), param->GenericGetVal< ValType >() );
    }

    //FIXME: this API should  be moved to some helper class as it looks shitty ang pretty uglu over here (as if PdrShader was responsible mostly for enabling samplers and textures)
    int     EnableTextureSamplers   ( Renderer * renderer );
    int     EnableTextureSamplers   ( Renderer * renderer, Shader * shader, int firstAvailableSamplerIndex );
    int     EnableTextureSamplers   ( Renderer * renderer, const std::vector< const TextureSampler * > & samplers, const ShaderTextureParameters & txParams, int firstAvailableSamplerIndex );

    //FIXME: implement other texture types
    void    EnableTextureSampler    ( Renderer * renderer, const TextureSampler * sampler, const Texture2D * texture, int samplerNum );
    void    EnableTextureSampler    ( Renderer * renderer, const TextureSampler * sampler, const TextureAnimatedSequence2D * animation, int samplerNum );

    int     DisableTextureSamplers  ( Renderer * renderer );
    int     DisableTextureSamplers  ( Renderer * renderer, Shader * shader, int firstAvailableSamplerIndex );
    int     DisableTextureSamplers  ( Renderer * renderer, const std::vector< const TextureSampler * > & samplers, const ShaderTextureParameters & txParams, int firstAvailableSamplerIndex );

    //FIXME: implement other texture types
    void    DisableTextureSampler   ( Renderer * renderer, const TextureSampler * sampler, const Texture2D * texture, int samplerNum );
    void    DisableTextureSampler   ( Renderer * renderer, const TextureSampler * sampler, const TextureAnimatedSequence2D * animation, int samplerNum );
    
};

} //bv
