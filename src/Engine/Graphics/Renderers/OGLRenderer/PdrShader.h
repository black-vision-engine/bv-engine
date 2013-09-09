#pragma once

#include "glslprogram.h"
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

class PdrShader
{
private:

    GLSLProgram *       m_program;

    PixelShader *       m_pixelShader;
    VertexShader *      m_vertexShader;
    GeometryShader *    m_geometryShader;

private:

    PdrShader   ( GLSLProgram * program, PixelShader * ps, VertexShader * vs, GeometryShader * gs );

public:

    ~PdrShader  ();

    static PdrShader *  Create( PixelShader * ps, VertexShader * vs, GeometryShader * gs );

public:

    void Enable         ( Renderer * renderer );
    void Disable        ( Renderer * renderer );

    //FIXME: add when textures are more or less implemented
    //void SetSamplerState (Renderer* renderer, RendererData::SamplerState * curSamplerState );
    //void DisableTextures (Renderer* renderer );

private:

    void    SetUniforms     ( Shader * shader );
    void    SetUniformParam ( GenericShaderParam * param );
    
    template< ParamType paramType >
    void    SetUniformDispatcher( GenericShaderParam * param )
    {
        typedef ShaderParamTypeMapper< paramType >::type ValType;
        m_program->SetUniform( param->Name().c_str(), param->GenericGetVal< ValType >() );
    }
};
}
