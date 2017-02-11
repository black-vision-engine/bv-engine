#include "stdafx.h"

#include "PdrShader.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Shaders/GeometryShader.h"

#include "Engine/Graphics/Shaders/TextureSampler.h"
#include "Engine/Graphics/Resources/Textures/Texture.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

//FIXME: implement those textures
//#include "Engine/Graphics/Resources/Texture1D.h"
#include "Engine/Graphics/Resources/Textures/Texture2D.h"
//#include "Engine/Graphics/Resources/Texture3D.h"
//#include "Engine/Graphics/Resources/TextureCubic.h"




#include "Memory/MemoryLeaks.h"



namespace bv 
{

// *******************************
//
PdrShader::PdrShader   ( PdrGLSLProgram * program, PixelShader * ps, VertexShader * vs, GeometryShader * gs )
    : m_program( program )
    , m_pixelShader( ps )
    , m_vertexShader( vs )
    , m_geometryShader( gs )
{
    m_program->Use();

    InitParamsLocations( ps );
    InitParamsLocations( vs );
    InitParamsLocations( gs );
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

    PdrGLSLProgram * program = new PdrGLSLProgram( *ps, *vs, gs );

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
    SetUniforms( m_geometryShader );
    SetUniforms( m_pixelShader );

    //FIXME: possibly use numSamplers somehow (debug and/or logging)
    int numSamplers = EnableTextureSamplers( renderer );
    { numSamplers; } // FIXME: suppress unused warning
    //m_program->PrintActiveUniforms();
    //m_program->PrintActiveAttribs();
}

// *******************************
//
void PdrShader::Disable        ( Renderer * renderer )
{
    DisableTextureSamplers( renderer );
}

// *******************************
//
void    PdrShader::BindUniformBuffer    ( PdrUniformBufferObject * ubo )
{
    if( ubo )
    {
        ubo->UniformBlockBinding( m_program->GetHandle() );
    }
}

// *******************************
// FIXME: if enabled or something should be used here
void    PdrShader::SetUniforms     ( Shader * shader )
{
    if ( shader )
    {
        ShaderParameters * params = shader->GetParameters();

        for ( unsigned int i = 0; i < params->NumParameters(); ++i )
        {
            auto param = params->GetParam( i );

            //FIXME: maybe this is not necessary, but for the time being may stay here (until active uniforms set is added or something similar is implemented)
            if ( param->IntID() >= 0 )
            {
                SetUniformParam( param );
            }
        }
    }
}

// *******************************
//FIXME: reimplement this method so that switch-case statement is not called here but rather in GenericParameter or even better - in shader desc
// FIXME: if enabled or something should be used here
void     PdrShader::InitSetUniformParam     ( const GenericShaderParam * param )
{
    switch( param->Type() )
    {
        case ParamType::PT_FLOAT1:
            InitSetUniform< ParamType::PT_FLOAT1 >( param );
            break;
        case ParamType::PT_FLOAT2:
            InitSetUniform< ParamType::PT_FLOAT2 >( param );
            break;
        case ParamType::PT_FLOAT3:
            InitSetUniform< ParamType::PT_FLOAT3 >( param );
            break;
        case ParamType::PT_FLOAT4:
            InitSetUniform< ParamType::PT_FLOAT4 >( param );
            break;
        case ParamType::PT_MAT2:
            InitSetUniform< ParamType::PT_MAT2 >( param );
            break;
        case ParamType::PT_MAT3:
            InitSetUniform< ParamType::PT_MAT3 >( param );
            break;
        case ParamType::PT_MAT4:
            InitSetUniform< ParamType::PT_MAT4 >( param );
            break;
        case ParamType::PT_INT:
            InitSetUniform< ParamType::PT_INT >( param );
            break;
        case ParamType::PT_BOOL:
            InitSetUniform< ParamType::PT_BOOL >( param );
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
    if( shader != nullptr )
    {
        auto & samplers = shader->Samplers();
        auto params = shader->GetParameters();

        assert( samplers.size() == params->NumTextures() );
        for( unsigned int i = 0; i < samplers.size(); ++i )
        {
            auto samplerParams = params->GetSamplerParameters( i );

            if( samplerParams )
            {
                samplers[ i ]->SetWrappingMode( samplerParams->GetWrappingModeX(), SamplerWrapDirection::SWD_S );
                samplers[ i ]->SetWrappingMode( samplerParams->GetWrappingModeY(), SamplerWrapDirection::SWD_T );
                samplers[ i ]->SetWrappingMode( samplerParams->GetWrappingModeZ(), SamplerWrapDirection::SWD_R );
                samplers[ i ]->SetFilteringMode( samplerParams->GetFilteringMode() );
                samplers[ i ]->SetBorderColor( samplerParams->GetBorderColor() );
            }

            EnableTextureSampler( renderer, samplers[ i ], params->GetTexture( i ).get(), i + firstAvailableSamplerIndex );

        }

        return (int) samplers.size();
    }

    return 0;
}

// *******************************
//
void    PdrShader::EnableTextureSampler    ( Renderer * renderer, const TextureSampler * sampler, const Texture2D * texture, int samplerNum )
{
    int textureUnit = samplerNum;
    bool textureUnitStateChanged = true;

    //FIXME: assert that texture type corresponds to sampler type
    switch( sampler->SamplingMode() )
    {
        case SamplerSamplingMode::SSM_MODE_1D:
        {
            renderer->Enable( texture, textureUnit );

            GLint wrap_s = (GLint) ConstantsMapper::GLConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_S ) );
            
			auto minFilteringMode = sampler->FilteringMode();
			auto magFilteringMode = sampler->FilteringMode();
			if( magFilteringMode > SamplerFilteringMode::SFM_LINEAR )
			{
				magFilteringMode = SamplerFilteringMode( int( magFilteringMode ) % 2 );
			}

            //FIXME: think a bit more about how filtering mag/min (and mipmaps) should be implemented
            GLint min_filter = (GLint) ConstantsMapper::GLConstant( minFilteringMode );
            GLint mag_filter = (GLint) ConstantsMapper::GLConstant( magFilteringMode );

            BVGL::bvglTexParameteri ( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, wrap_s );

            BVGL::bvglTexParameteri ( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, min_filter );
            BVGL::bvglTexParameteri ( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, mag_filter );

            BVGL::bvglTexParameterfv( GL_TEXTURE_1D, GL_TEXTURE_BORDER_COLOR, &sampler->GetBorderColor()[ 0 ] );

            break;
        }
        case SamplerSamplingMode::SSM_MODE_2D:
        {
            if ( renderer->IsEnabled( texture, textureUnit ) )
            {
                textureUnitStateChanged = false;
            }

            // FIXME: does not enable texture but calls Update on the texture (which is bound - textureUnit binding is not touched)
            renderer->Enable( texture, textureUnit );

            //FIXME: this state may be cached in currentSamplerState in Renderer (for specified target (GL_TEXTURE_2D here and selected texturing unit)
            GLint wrap_s = (GLint) ConstantsMapper::GLConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_S ) );
            GLint wrap_t = (GLint) ConstantsMapper::GLConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_T ) );
            
			auto minFilteringMode = sampler->FilteringMode();
			auto magFilteringMode = sampler->FilteringMode();
			if( magFilteringMode > SamplerFilteringMode::SFM_LINEAR )
			{
				magFilteringMode = SamplerFilteringMode( int( magFilteringMode ) % 2 );
			}

            //FIXME: think a bit more about how filtering mag/min (and mipmaps) should be implemented
            GLint min_filter = (GLint) ConstantsMapper::GLConstant( minFilteringMode );
            GLint mag_filter = (GLint) ConstantsMapper::GLConstant( magFilteringMode );

            BVGL::bvglTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s );
            BVGL::bvglTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t );

            BVGL::bvglTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter );
            BVGL::bvglTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter );

            BVGL::bvglTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &sampler->GetBorderColor()[ 0 ] );

            //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy ); //FIXME: when anisotropy is implemented in texture sampler
            //glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, lodBias);          //FIXME: when lodbias is implemented in texture sampler

            break;
        }
        case SamplerSamplingMode::SSM_MODE_3D:
        {
            renderer->Enable( texture, textureUnit );

            GLint wrap_s = (GLint) ConstantsMapper::GLConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_S ) );
            GLint wrap_t = (GLint) ConstantsMapper::GLConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_T ) );
            GLint wrap_r = (GLint) ConstantsMapper::GLConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_R ) );
            
			auto minFilteringMode = sampler->FilteringMode();
			auto magFilteringMode = sampler->FilteringMode();
			if( magFilteringMode > SamplerFilteringMode::SFM_LINEAR )
			{
				magFilteringMode = SamplerFilteringMode( int( magFilteringMode ) % 2 );
			}

            //FIXME: think a bit more about how filtering mag/min (and mipmaps) should be implemented
            GLint min_filter = (GLint) ConstantsMapper::GLConstant( minFilteringMode );
            GLint mag_filter = (GLint) ConstantsMapper::GLConstant( magFilteringMode );

            BVGL::bvglTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap_s );
            BVGL::bvglTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap_t );
            BVGL::bvglTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap_r );

            BVGL::bvglTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, min_filter );
            BVGL::bvglTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, mag_filter );

            BVGL::bvglTexParameterfv( GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, &sampler->GetBorderColor()[ 0 ] );

            break;
        }
        case SamplerSamplingMode::SSM_MODE_CUBIC:
        default:
            assert( false );

    }

    int loc = sampler->GetCachedLoc();

    if( loc >= 0 )
    {
        if( textureUnitStateChanged || textureUnit != renderer->GetSamplerTexUnit( loc ) )
        {
			renderer->SetDisabled( texture, textureUnit );
			
			renderer->Enable( texture, textureUnit );

            m_program->SetUniform( loc, textureUnit );

            renderer->SetSamplerTexUnit( loc, textureUnit );
        }
    }
    else
    {
        loc = m_program->SetUniform( sampler->GetName().c_str(), textureUnit );

        sampler->CacheLoc( loc );

        renderer->SetSamplerTexUnit( loc, textureUnit );
    }
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
    if ( shader != nullptr )
    {
        auto samplers = shader->Samplers();
        auto params = shader->GetParameters();

        for( unsigned int i = 0; i < samplers.size(); ++i )
        {
            DisableTextureSampler( renderer, samplers[ i ], params->GetTexture( i ).get(), i + firstAvailableSamplerIndex );
        }

        return (int) samplers.size();
    }

    return 0;
}

// *******************************
//
void    PdrShader::DisableTextureSampler   ( Renderer * renderer, const TextureSampler * sampler, const Texture2D * texture, int samplerNum )
{
    int textureUnit = samplerNum;

    //FIXME: assert that texture type corresponds to sampler type
    switch( sampler->SamplingMode() )
    {
        case SamplerSamplingMode::SSM_MODE_2D:
        {
            renderer->Disable( texture, textureUnit );
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

// *******************************
//
void    PdrShader::InitParamsLocations     ( Shader * shader )
{
    if( shader )
    {
        ShaderParameters * params = shader->GetParameters();

        for( unsigned int i = 0; i < params->NumParameters(); ++i )
        {
            const GenericShaderParam * param = params->GetParam( i );
            InitSetUniformParam( param );
        }
    }
}

} //bv
