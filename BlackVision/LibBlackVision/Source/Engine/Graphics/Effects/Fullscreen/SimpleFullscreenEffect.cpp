#include "SimpleFullscreenEffect.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Effects/Utils/FullscreenUtils.h"
#include "Engine/Graphics/Effects/FullScreen/FullscreenVSShader.h"
#include "Engine/Graphics/Effects/FullScreen/FullscreenRenderableEffectTr.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Models/Builder/RendererStatesBuilder.h"


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
    renderer->Enable( ctx->GetOutputRenderTarget() );
    
    SynchronizeInputData( ctx );

    renderer->Draw( m_fullscreenQuad );

    renderer->Disable( ctx->GetOutputRenderTarget() );
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
    auto startIdx = ctx->GetFirstRenderTargetIndex();

    assert( rtVec != nullptr );

    if( m_data.GetNumInitializedTextures() < m_data.GetNumTextures() )
    {
        assert( m_data.GetNumTextures() <= (unsigned int) ( rtVec->size() - startIdx ) );
        
        for( unsigned int i = 0; i < m_data.GetNumTextures(); ++i )
        {
            m_data.SetInputTexture( (*rtVec)[ i + startIdx ]->ColorTexture( 0 ), i );
        }
    }
    else
    {
        assert( DebugVerifyInput( rtVec, startIdx ) );
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

    auto effect = std::make_shared< FullscreenRenderableEffectTr >( pass );

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
    as->blendEnabled = false;
    ds->enabled = false;
    cs->enabled = false;

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

} //bv