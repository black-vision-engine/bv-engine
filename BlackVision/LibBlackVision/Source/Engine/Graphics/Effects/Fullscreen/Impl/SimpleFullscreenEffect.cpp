#include "stdafx.h"

#include "SimpleFullscreenEffect.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Effects/Utils/FullscreenUtils.h"
#include "Engine/Graphics/Effects/FullScreen/Impl/FullscreenVSShader.h"
#include "Engine/Graphics/Effects/FullScreen/Impl/FullscreenRenderableEffect.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamInt.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamFloat.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec2.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec3.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec4.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamMat2.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamMat3.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamMat4.h"

#include "Engine/Models/Interfaces/ITextureDescriptor.h"

#include "UseLoggerLibBlackVision.h"

namespace  bv {

// **************************
//
SimpleFullscreenEffect::SimpleFullscreenEffect  ( const FullscreenEffectData & inputData )
    : m_data( inputData )
    , m_fullscreenCamera( nullptr )
    , m_fullscreenQuad( nullptr )
{
    m_fullscreenCamera = FullscreenUtils::CreateDisplayCamera();

    m_fullscreenQuad = CreateFullscreenQuad( inputData );
}

// **************************
//
SimpleFullscreenEffect::~SimpleFullscreenEffect ()
{
    delete m_fullscreenCamera;
    delete m_fullscreenQuad;
}

// **************************
//
void    SimpleFullscreenEffect::Render          ( FullscreenEffectContext * ctx )
{
    auto renderer = ctx->GetRenderer();

    auto rendererCamera = renderer->GetCamera();
    renderer->SetCamera( m_fullscreenCamera );
    //FIXME: decide if this RT should be bound or not - ion simple effect it may not be the case, but consider composite effects - either inner effects or graph implementation
    //FIXME: would have to take care of setting valid render target
    // renderer->Enable( ctx->GetOutputRenderTarget() );

    SynchronizeInputData( ctx );

    renderer->Draw( m_fullscreenQuad );

    //FIXME: decide if this RT should be bound or not - ion simple effect it may not be the case, but consider composite effects - either inner effects or graph implementation
    //FIXME: would have to take care of setting valid render target
    // renderer->Disable( ctx->GetOutputRenderTarget() );
    renderer->SetCamera( rendererCamera );
}

// **************************
//
unsigned int    SimpleFullscreenEffect::GetNumInputs    () const
{
    return m_data.GetNumTextures();
}

// **************************
//
void                SimpleFullscreenEffect::SynchronizeInputData    ( FullscreenEffectContext * ctx )
{
    auto rtVec = ctx->AccessInputRenderTargets();
    
    if( rtVec != nullptr )
    {
        auto startIdx = ctx->GetFirstRenderTargetIndex();

        assert( rtVec != nullptr );

        if( m_data.GetNumInitializedTextures() < m_data.GetNumTextures() || ctx->IsSyncRequired() )
        {
            assert( m_data.GetNumTextures() <= (unsigned int) ( rtVec->size() - startIdx ) );
        
            auto effect = m_fullscreenQuad->GetRenderableEffect();
            auto pass = effect->GetPass( 0 );
            auto ps = pass->GetPixelShader();
            auto psParams = ps->GetParameters();

            for( unsigned int i = 0; i < m_data.GetNumTextures(); ++i )
            {
                auto texture = (*rtVec)[ i + startIdx ]->ColorTexture( 0 );

                m_data.SetInputTexture( texture, i );
                psParams->SetTexture( i, texture );
            }
        }
        else
        {
            assert( DebugVerifyInput( rtVec, startIdx ) );
        }
    }
}

// **************************
//
std::vector< IValuePtr >    SimpleFullscreenEffect::GetValues       () const
{
    return m_data.GetValues();
}

// **************************
//
TriangleStrip *     SimpleFullscreenEffect::CreateFullscreenQuad    ( const FullscreenEffectData & inputData ) const
{
    auto effect = CreateEffect( inputData );

    auto quad =  FullscreenUtils::CreateFullscreenQuad( effect, 1 );

    return quad;
}

// **************************
//
RenderableEffectPtr SimpleFullscreenEffect::CreateEffect            ( const FullscreenEffectData & inputData ) const
{
    auto vs = CreateVertexShader( inputData );
    auto ps = CreatePixelShader( inputData );

    RenderablePass * pass = CreateRenderablePass( ps, vs, inputData );

    auto effect = std::make_shared< FullscreenRenderableEffect >( pass );

    return effect;
}

// **************************
//
VertexShader *      SimpleFullscreenEffect::CreateVertexShader      ( const FullscreenEffectData & inputData ) const
{
    { inputData; }

    auto params     = new ShaderParameters();
    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();

    params->AddParameter( mvpParam );

    auto source = FullscreenVSShader::GenerateDefaultVS( 1 );

    auto shader = new VertexShader( source, params );

    return shader;
}

// **************************
//
PixelShader *       SimpleFullscreenEffect::CreatePixelShader       ( const FullscreenEffectData & inputData ) const
{
    PixelShader * shader = nullptr;

    ShaderParameters * shaderParams = new ShaderParameters();
    
    for( unsigned int i = 0; i < inputData.GetNumValues(); ++i )
    {
        auto param = ShaderParamFactory::CreateGenericParameter( inputData.GetValueAt( i ).get() );

        shaderParams->AddParameter( param );
    }

    for( unsigned int i = 0; i < inputData.GetNumTextures(); ++i )
    {
        // nullptr textures are allowed
        shaderParams->AddTexture( inputData.GetInputTextureAt( i ) );
    }

    shader = new PixelShader( inputData.GetPixelShaderSource(), shaderParams );

    for( unsigned int i = 0; i < inputData.GetNumTextures(); ++i )
    {
        auto sampler = CreateSampler( inputData.GetSamplerNameAt( i ) );

        shader->AddTextureSampler( sampler );
    }

    return shader;
}

// **************************
//
RenderablePass *    SimpleFullscreenEffect::CreateRenderablePass    ( PixelShader * ps, VertexShader * vs, const FullscreenEffectData & inputData ) const
{
    { inputData; }

    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );

    auto sinst = pass->GetStateInstance();
    
    RendererStatesBuilder::Create( sinst );

    auto as = RenderStateAccessor::AccessAlphaState( sinst );
    auto ds = RenderStateAccessor::AccessDepthState( sinst );
    auto cs = RenderStateAccessor::AccessCullState( sinst );

    //FIXME: read it from the inputData
    as->blendEnabled = inputData.IsBlendEnabled();
    ds->enabled = inputData.IsDepthTestEnabled();
    cs->enabled = inputData.IsCullEnabled();

    return pass;
}

// **************************
//
TextureSampler *    SimpleFullscreenEffect::CreateSampler           ( const std::string & samplerName ) const
{
    auto wrapX          = EngineConstantsMapper::EngineConstant( TextureWrappingMode::TWM_CLAMP );
    auto wrapY          = EngineConstantsMapper::EngineConstant( TextureWrappingMode::TWM_CLAMP );            
    auto samplingMode   = SamplerSamplingMode::SSM_MODE_2D;
    auto sfm            = EngineConstantsMapper::EngineConstant( TextureFilteringMode::TFM_POINT );

    SamplerWrappingMode wrappingMode[] = { wrapX, wrapY, SamplerWrappingMode::SWM_REPEAT };

    auto sampler = new TextureSampler( 0, samplerName, samplingMode, sfm, wrappingMode, glm::vec4( 0.f, 0.f, 0.f, 0.f ) ); 

    return sampler;
}

// **************************
//
bool                SimpleFullscreenEffect::DebugVerifyInput        ( const std::vector< RenderTarget * > * rtVec, unsigned int startIdx )
{
    assert( rtVec != nullptr );

    assert( m_data.GetNumInitializedTextures() == m_data.GetNumTextures() );
    assert( m_data.GetNumTextures() <= (unsigned int) ( rtVec->size() - startIdx ) );

    bool success = true;

    for( unsigned int i = 0; i < m_data.GetNumTextures(); ++i )
    {
        success &= m_data.GetInputTextureAt( i ) == (*rtVec)[ i + startIdx ]->ColorTexture( 0 );
    }

    return success;
}

namespace {

// *******************************
//
template< typename ValType, typename ShaderParamType >
void	UpdateTypedShaderParam   ( IValueConstPtr source, GenericShaderParam * dest )
{
	static_cast< ShaderParamType * >( dest )->SetValue( QueryTypedValue< ValType >( source )->GetValue() );
}

// *****************************
//
void            UpdateShaderParam				( IValueConstPtr source, GenericShaderParam * dest )
{
	assert( source && source->GetType() == dest->Type() );
	
	switch( source->GetType() )
	{
	case ParamType::PT_FLOAT1:
		UpdateTypedShaderParam< ValueFloatPtr, ShaderParamFloat >( source, dest );
		break;
	case ParamType::PT_INT:
		UpdateTypedShaderParam< ValueIntPtr, ShaderParamInt >( source, dest );
		break;
	case ParamType::PT_FLOAT2:
		UpdateTypedShaderParam< ValueVec2Ptr, ShaderParamVec2 >( source, dest );
		break;
	case ParamType::PT_FLOAT3:
		UpdateTypedShaderParam< ValueVec3Ptr, ShaderParamVec3 >( source, dest );
		break;
	case ParamType::PT_FLOAT4:
		UpdateTypedShaderParam< ValueVec4Ptr, ShaderParamVec4 >( source, dest );
		break;
	case ParamType::PT_MAT2:
		UpdateTypedShaderParam< ValueMat2Ptr, ShaderParamMat2 >( source, dest );
		break;
	case ParamType::PT_MAT3:
		UpdateTypedShaderParam< ValueMat3Ptr, ShaderParamMat3 >( source, dest );
		break;
	case ParamType::PT_MAT4:
		UpdateTypedShaderParam< ValueMat4Ptr, ShaderParamMat4 >( source, dest );
		break;
	}
}
}
// **************************
//
void                SimpleFullscreenEffect::Update              ()
{
    for( auto v : GetValues() ) 
    {
        auto name = v->GetName();
        auto param = m_fullscreenQuad->GetRenderableEffect()->GetPass( 0 )->GetPixelShader()->GetParameters()->AccessParam( name );

        if( param == nullptr ) 
        {
            param = m_fullscreenQuad->GetRenderableEffect()->GetPass( 0 )->GetVertexShader()->GetParameters()->AccessParam( name );
        }

        if( param == nullptr ) 
        {
            param = m_fullscreenQuad->GetRenderableEffect()->GetPass( 0 )->GetGeometryShader()->GetParameters()->AccessParam( name );
        }

        if( param != nullptr ) 
        {
            UpdateShaderParam( v, param );
        }
    }
}

// **************************
//
void            SimpleFullscreenEffect::AddTexture   ( const ITextureDescriptorConstPtr & txDesc )
{
    auto tx2d = std::make_shared< Texture2D >( txDesc->GetFormat(), txDesc->GetWidth(), txDesc->GetHeight(), txDesc->GetSemantic(), txDesc->GetNumLevels() );

    for( UInt32 i = 0; i < txDesc->GetNumLevels(); ++i )
    {
        tx2d->SetData( txDesc->GetBits( i ), i );
    }

    m_data.AppendInputTexture( tx2d, txDesc->GetName() );
}

} //bv
