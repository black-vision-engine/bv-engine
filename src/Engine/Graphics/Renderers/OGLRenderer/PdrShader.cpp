#include "PdrShader.h"
#include "glslprogram.h"

#include "Engine\Graphics\Renderers\Renderer.h"

#include "Engine/Graphics/Shaders/ShaderParam.h"
#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Shaders/GeometryShader.h"

#include "Engine\Graphics\Shaders\TextureSampler.h"
#include "Engine\Graphics\Resources\Texture.h"

#include "Engine\Graphics\Renderers\OGLRenderer\PdrConstants.h"

//FIXME: implement those textures
//#include "Engine\Graphics\Resources\Texture1D.h"
#include "Engine\Graphics\Resources\Texture2D.h"
//#include "Engine\Graphics\Resources\Texture3D.h"
//#include "Engine\Graphics\Resources\TextureCubic.h"

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
    //FIXME: some API for location binding should be (most probably) exposed here as well
    m_program->Use();

    SetUniforms( m_vertexShader );
    SetUniforms( m_pixelShader );
    SetUniforms( m_geometryShader );

    //FIXME: possibly use numSamplers somehow (debug and/or logging)
    int numSamplers = EnableTextureSamplers( renderer );

    m_program->PrintActiveUniforms();
    m_program->PrintActiveAttribs();
}

// *******************************
//
void PdrShader::Disable        ( Renderer * renderer )
{
    DisableTextureSamplers( renderer );
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

// *******************************
//
int     PdrShader::EnableTextureSamplers   ( Renderer * renderer )
{
    int numSamplers = 0;

    numSamplers += EnableTextureSamplers( renderer, m_pixelShader, numSamplers );
    numSamplers += EnableTextureSamplers( renderer, m_vertexShader, numSamplers );
    numSamplers += EnableTextureSamplers( renderer, m_geometryShader, numSamplers );

    return numSamplers;
}

// *******************************
//
int     PdrShader::EnableTextureSamplers   ( Renderer * renderer, Shader * shader, int firstAvailableSamplerIndex )
{
    if ( shader == nullptr )
    {
        return 0;
    }

    return EnableTextureSamplers( renderer, shader->Samplers(), shader->GetOrCreateShaderParameters()->Textures(), firstAvailableSamplerIndex );
}

// *******************************
//
int      PdrShader::EnableTextureSamplers   ( Renderer * renderer, const std::vector< const TextureSampler * > & samplers, const std::vector< Texture * > & textures, int firstAvailableSamplerIndex )
{
    assert( samplers.size() == textures.size() );
    
    for( unsigned int i = 0; i < samplers.size(); ++i )
    {
        EnableTextureSampler( renderer, samplers[ i ], textures[ i ], i + firstAvailableSamplerIndex );
    }

    return samplers.size();
}

// *******************************
//
void    PdrShader::EnableTextureSampler    ( Renderer * renderer, const TextureSampler * sampler, const Texture * texture, int samplerNum )
{
    int textureUnit = samplerNum;

    //FIXME: assert that texture type corresponds to sampler type
    switch( sampler->SamplingMode() )
    {
        case SamplerSamplingMode::SSM_MODE_2D:
        {
            renderer->Enable( static_cast< const Texture2D * >( texture ), textureUnit );

            //FIXME: this state may be cached in currentSamplerState in Renderer (for specified target (GL_TEXTURE_2D here and selected texturing unit)
            GLint wrap_s = (GLint) ConstantsMapper::GlConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_S ) );
            GLint wrap_t = (GLint) ConstantsMapper::GlConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_T ) );
            
            //FIXME: think a bit more about how filtering mag/min (and mipmaps) should be implemented
            GLint min_filter = (GLint) ConstantsMapper::GlConstant( sampler->FilteringMode() );
            GLint mag_filter = (GLint) ConstantsMapper::GlConstant( sampler->FilteringMode() );

            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t );

            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter );
            glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mag_filter );

            glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &sampler->GetBorderColor()[ 0 ] );

            //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy ); //FIXME: when anisotropy is implemented in texture sampler
            //glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, lodBias);          //FIXME: when lodbias is implemented in texture sampler

            break;
        }
        //FIXME: implement        
        case SamplerSamplingMode::SSM_MODE_1D:
        case SamplerSamplingMode::SSM_MODE_3D:
        case SamplerSamplingMode::SSM_MODE_CUBIC:
        default:
            assert( false );

    }

    m_program->SetUniform( sampler->GetName().c_str(), textureUnit );
}

// *******************************
//
int     PdrShader::DisableTextureSamplers  ( Renderer * renderer )
{
    int numSamplers = 0;

    numSamplers += DisableTextureSamplers( renderer, m_geometryShader, numSamplers );
    numSamplers += DisableTextureSamplers( renderer, m_vertexShader, numSamplers );
    numSamplers += DisableTextureSamplers( renderer, m_pixelShader, numSamplers );

    return numSamplers;
}

// *******************************
//
int    PdrShader::DisableTextureSamplers  ( Renderer * renderer, Shader * shader, int firstAvailableSamplerIndex )
{
    if ( shader == nullptr )
    {
        return 0;
    }

    return DisableTextureSamplers( renderer, shader->Samplers(), shader->GetOrCreateShaderParameters()->Textures(), firstAvailableSamplerIndex );
}

// *******************************
//
int     PdrShader::DisableTextureSamplers  ( Renderer * renderer, const std::vector< const TextureSampler * > & samplers, const std::vector< Texture * > & textures, int firstAvailableSamplerIndex )
{
    for( unsigned int i = 0; i < samplers.size(); ++i )
    {
        DisableTextureSampler( renderer, samplers[ i ], textures[ i ], i + firstAvailableSamplerIndex );
    }

    return samplers.size();
}

// *******************************
//
void    PdrShader::DisableTextureSampler   ( Renderer * renderer, const TextureSampler * sampler, const Texture * texture, int samplerNum )
{
    int textureUnit = samplerNum;

    //FIXME: assert that texture type corresponds to sampler type
    switch( sampler->SamplingMode() )
    {
        case SamplerSamplingMode::SSM_MODE_2D:
        {
            renderer->Disable( static_cast< const Texture2D * >( texture ), textureUnit );
            break;
        }
        //FIXME: implement        
        case SamplerSamplingMode::SSM_MODE_1D:
        case SamplerSamplingMode::SSM_MODE_3D:
        case SamplerSamplingMode::SSM_MODE_CUBIC:
        default:
            assert( false );

    }
}

} //bv
