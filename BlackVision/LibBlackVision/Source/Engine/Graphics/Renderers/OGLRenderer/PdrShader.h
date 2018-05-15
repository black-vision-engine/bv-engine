#pragma once

#include "Mathematics/glm_inc.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrGLSLProgram.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrUniformBufferObject.h"
#include "Engine/Graphics/Shaders/Parameters/GenericShaderParam.h"
#include "Engine/Types/Enums.h"


namespace bv
{

class Renderer;
class Shader;
class PixelShader;
class VertexShader;
class GeometryShader;
class TextureSampler;

class Texture2D;
class TextureAnimatedSequence2D;

class ShaderParameters;

class PdrShader
{
private:

    PdrGLSLProgram *    m_program;

    PixelShader *       m_pixelShader;
    VertexShader *      m_vertexShader;
    GeometryShader *    m_geometryShader;

private:

    PdrShader                       ( PdrGLSLProgram * program, PixelShader * ps, VertexShader * vs, GeometryShader * gs );

public:

    ~PdrShader                      ();

    static PdrShader *  Create      ( PixelShader * ps, VertexShader * vs, GeometryShader * gs );

public:

    void    Enable                  ( Renderer * renderer );
    void    Disable                 ( Renderer * renderer );

    //FIXME: use SamplerState to cache it at least a bit (this should be stored in current Renderer)
    //void SetSamplerState (Renderer* renderer, RendererData::SamplerState * curSamplerState ); //see comments in PdrShader.cpp

    void    BindUniformBuffer       ( PdrUniformBufferObject * ubo );

private:

    void        SetUniforms         ( Shader * shader );
    inline void SetUniformParam     ( const GenericShaderParam * param );
    void        InitSetUniformParam ( const GenericShaderParam * param );

    //FIXME: this API should  be moved to some helper class as it looks ugly over here (as if PdrShader was responsible mostly for enabling samplers and textures)
    int     EnableTextureSamplers   ( Renderer * renderer );
    int     EnableTextureSamplers   ( Renderer * renderer, Shader * shader, int firstAvailableSamplerIndex );

    //FIXME: implement additional texture types
    void    EnableTextureSampler    ( Renderer * renderer, const TextureSampler * sampler, const Texture2D * texture, int samplerNum );

    int     DisableTextureSamplers  ( Renderer * renderer );
    int     DisableTextureSamplers  ( Renderer * renderer, Shader * shader, int firstAvailableSamplerIndex );

    //FIXME: implement additioonal texture types
    void    DisableTextureSampler   ( Renderer * renderer, const TextureSampler * sampler, const Texture2D * texture, int samplerNum );
    
    void    InitParamsLocations     ( Shader * shader );

    template< ParamType paramType >
    inline void InitSetUniform      ( const GenericShaderParam * param );

    template< ParamType paramType >
    inline void SetUniform          ( const GenericShaderParam * param );

};

} //bv

#include "PdrShader.inl"
