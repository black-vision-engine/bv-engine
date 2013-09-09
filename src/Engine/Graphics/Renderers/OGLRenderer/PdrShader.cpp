#include "PdrShader.h"
#include "glslprogram.h"

#include "Engine/Graphics/Shaders/ShaderParam.h"
#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Shaders/GeometryShader.h"

namespace bv 
{

// *******************************
//
PdrShader::PdrShader   ( GLSLProgram * program, PixelShader * ps, VertexShader * vs, GeometryShader * gs )
    : m_program( program )
    , m_pixelShader( ps )
    , m_vertexShader( vs )
    , m_geometryShader( gs )
{
}

// *******************************
//
PdrShader::~PdrShader  ()
{
    delete m_program;
}

// *******************************
//
PdrShader *  PdrShader::Create( PixelShader * ps, VertexShader * vs, GeometryShader * gs )
{
    if ( ps == nullptr || vs == nullptr )
    {
        return nullptr;
    }

    GLSLProgram * program = new GLSLProgram( *ps, *vs, gs );

    if ( !program->IsCompiled() )
    {
        delete program;
        return nullptr;
    }

    return new PdrShader( program, ps, vs, gs );
}

// *******************************
//
void PdrShader::Enable         ( Renderer * renderer )
{
    //FIXME: Enable textures
    //FIXME: some API for location binding should be (most probably) exposed here as well
    m_program->Use();

    SetUniforms( m_vertexShader );
    SetUniforms( m_pixelShader );
    SetUniforms( m_geometryShader );

    //m_program->PrintActiveUniforms();
    //m_program->PrintActiveAttribs();
}

// *******************************
//
void PdrShader::Disable        ( Renderer * renderer )
{
    //FIXME: Disable textures
}

// *******************************
// FIXME: if enabled or something should be used here
void    PdrShader::SetUniforms     ( Shader * shader )
{
    if ( shader )
    {
        ShaderParameters * params = shader->Parameters();

        for ( int i = 0; i < params->NumParameters(); ++i )
        {
            SetUniformParam( params->GetParam( i ) );
        }
    }
}

// *******************************
//FIXME: reimplement this method so that switch-case statement is not called here but rather in GenericParameter or even better - in shader desc
void    PdrShader::SetUniformParam ( GenericShaderParam * param )
{
    switch( param->Type() )
    {
        case ParamType::PT_FLOAT:
            SetUniformDispatcher< ParamType::PT_FLOAT >( param );
            break;
        case ParamType::PT_FLOAT1:
            SetUniformDispatcher< ParamType::PT_FLOAT1 >( param );
            break;
        case ParamType::PT_FLOAT2:
            SetUniformDispatcher< ParamType::PT_FLOAT2 >( param );
            break;
        case ParamType::PT_FLOAT3:
            SetUniformDispatcher< ParamType::PT_FLOAT3 >( param );
            break;
        case ParamType::PT_FLOAT4:
            SetUniformDispatcher< ParamType::PT_FLOAT4 >( param );
            break;
        case ParamType::PT_MAT3:
            SetUniformDispatcher< ParamType::PT_MAT3 >( param );
            break;
        case ParamType::PT_MAT4:
            SetUniformDispatcher< ParamType::PT_MAT4 >( param );
            break;
        case ParamType::PT_INT:
            SetUniformDispatcher< ParamType::PT_INT >( param );
            break;
        case ParamType::PT_BOOL:
            SetUniformDispatcher< ParamType::PT_BOOL >( param );
            break;
        default:
            assert( false );
            break;
    }
}

} //bv
