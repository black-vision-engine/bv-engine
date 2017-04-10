#include "stdafx.h"

#include "FulscreenEffectVisualComponent.h"

#include "Engine/Graphics/SceneGraph/TriangleStrip.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParameters.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/Impl/FullscreenVSShader.h"
#include "Engine/Graphics/Effects/Logic/FullscreenRendering/Impl/FullscreenEffectHelpers.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Models/Updaters/ShaderParamUpdater.h"


namespace bv { 

// **************************
// FIXME: nrl - values are passed here from FSE state which is not the best possible approach
// FIXME: nrl - only FSE shoould keep track of IValues and VisualComponent should be externally updated to make sure that its state is passed to ShaderParameters kept in this class
// FIXME: nrl - right now it is left as it is (state is updated before rendering form m_values) but remove valules from this constructor as soon as all FSEs are implemented
FullscreenEffectVisualComponent::FullscreenEffectVisualComponent    ( TriangleStrip * quad, unsigned int numRendenderTargetTextures, unsigned int numStaticTextures, const IValuePtrVec & values )
    : m_quad( quad )
    , m_numRenderTargetTextures( numRendenderTargetTextures )
    , m_numStaticTextures( numStaticTextures )
    , m_values( values )

{
    auto effect = m_quad->GetRenderableEffect();
    auto pass = effect->GetPass( 0 );

    m_pixelShader   = pass->GetPixelShader();

    auto shaderParams = m_pixelShader->GetParameters();

    assert( shaderParams->NumParameters() == values.size() );
    { shaderParams; }

    // FIXME: nrl - make sure (by asserting) that m_numRenderTargetTextures corresponds to m_numRenderTargetTextures nullptr entries in the pixel shader (as well as numStaticTextures corresponds to the following
    // FIXME: nrl - numStaticTextures entries with non nullptr entries corresponding to static textures)
}

// **************************
//
FullscreenEffectVisualComponent::~FullscreenEffectVisualComponent     ()
{
    delete m_quad;
}

// **************************
//
void    FullscreenEffectVisualComponent::Render                        ( Renderer * renderer )
{
    // At this point all input data must be synchronized.
    // This class is not responsible for tracking external sources of possible state changes.

    // FIXME: nrl - this one should be called from some other place (so that values don't have to be tracked by this class which should only care about its shader parameters)
    UpdateShaderParams();

    renderer->Draw( m_quad );
}

// **************************
//
void    FullscreenEffectVisualComponent::SyncRenderTargets             ( const RenderTargetVec & renderTargets )
{
    assert( (unsigned int) renderTargets.size() == m_numRenderTargetTextures );

    unsigned int k = FirstRenderTargetTextureIdx();

    for( unsigned int i = 0; i < m_numRenderTargetTextures; ++i, ++k )
    {
        auto tex = renderTargets[ i ]->ColorTexture( 0 );

        SyncTexture( k, tex );
    }
}

// **************************
//
void    FullscreenEffectVisualComponent::SyncTextures                  ( const Texture2DEntryVec & textures )
{
    assert( (unsigned int) textures.size() == m_numRenderTargetTextures );
    
    unsigned int k = FirstStaticTextureIdx();

    for( unsigned int i = 0; i < m_numRenderTargetTextures; ++i, ++k )
    {
        auto entry  = textures[ i ];

        SyncStaticTexture( k, entry );
    }
}

// **************************
//
void    FullscreenEffectVisualComponent::SyncRenderState               ( const FullscreenEffectRendererState & state )
{
    auto sinst = GetRenderStateInstance( m_quad );
    
    FullscreenEffectHelpers::SetRenderState( sinst, state );
}

// **************************
//
void    FullscreenEffectVisualComponent::SyncIValues                   ( const IValuePtrVec & vals )
{
    auto ps = GetPixelShader( m_quad );
    
    auto shaderParams = ps->GetParameters();

    assert( shaderParams->NumParameters() == vals.size() );

    unsigned int numUpdated = 0;

    // Create parameters
    for( auto val : vals )
    {
        auto param = shaderParams->AccessParam( val->GetName() );

        assert( param != nullptr );
		param;

        numUpdated++;

        //FIXME: nrl implement
        //param->UpdateValuePtr( val.get() );
    }

    // FIXME: this test does not include repeated values, but at this moment it doesn't seem too important
    assert( numUpdated == shaderParams->NumParameters() );
}

// **************************
//
const IValuePtrVec &    FullscreenEffectVisualComponent::GetIValues    () const
{
    return m_values;
}

// **************************
//
void                    FullscreenEffectVisualComponent::AppendRenderPasses_DIRTY_HACK       ( std::set< const RenderablePass * > * passes ) const
{
    auto eff = m_quad->GetRenderableEffect();

    for( unsigned int i = 0; i < eff->NumPasses(); ++i )
    {
        auto pass = eff->GetPass( i );

        passes->insert( pass );
    }
}

// **************************
//
FullscreenEffectVisualComponent *   FullscreenEffectVisualComponent::Create( const FullscreenEffectVisualComponentDesc & desc )
{
    auto & pixelShaderSource    = desc.GetPixelShaderSrc();
    auto & textures             = desc.GetStaticTextures();
    auto & values               = desc.GetValues();
    auto & rtInputSamplers      = desc.GetInputSamplers();
    auto & rendererState        = desc.GetRendererState();

    auto geom = FullscreenEffectHelpers::CreateFullscreenRenderable( pixelShaderSource, textures, values, rtInputSamplers, rendererState );

    unsigned int numRenderTargetTextures    = (unsigned int) rtInputSamplers.size();
    unsigned int numStaticTextures          = (unsigned int) textures.size();
    
    return new FullscreenEffectVisualComponent( geom, numRenderTargetTextures, numStaticTextures, values );
}

// **************************
//
void            FullscreenEffectVisualComponent::UpdateShaderParams                  ()
{
    auto shaderParams = m_pixelShader->GetParameters();

    unsigned int i = 0;

    for( auto & val : m_values )
    {
        auto param = shaderParams->AccessParam( i );

        assert( param->Name() == val->GetName() );

        UpdateGenericShaderParam( val.get(), param );

        ++i;
    }
}

// **************************
//
unsigned int    FullscreenEffectVisualComponent::FirstRenderTargetTextureIdx         () const
{
    return 0;
}

// **************************
//
unsigned int    FullscreenEffectVisualComponent::FirstStaticTextureIdx               () const
{
    return FirstRenderTargetTextureIdx() + m_numRenderTargetTextures;
}

// **************************
//
void            FullscreenEffectVisualComponent::SyncTexture                         ( unsigned int i, Texture2DPtr tex )
{
    auto params = m_pixelShader->GetParameters();

    params->SetTexture( i, tex );
}

// **************************
//
void            FullscreenEffectVisualComponent::SyncSampler                         ( unsigned int i, const std::string & name, TextureFilteringMode tfm, TextureWrappingMode wrapX, TextureWrappingMode wrapY )
{
    { name; } // FIXME: nrl - this is not used - only the relative ordering is taken into account

    auto & samplers = m_pixelShader->Samplers();

    auto sampler = samplers[ i ];

    assert( sampler->GetName() == name );

    sampler->SetWrappingMode( EngineConstantsMapper::EngineConstant( wrapX ), SamplerWrapDirection::SWD_S );
	sampler->SetWrappingMode( EngineConstantsMapper::EngineConstant( wrapY ), SamplerWrapDirection::SWD_T );
	sampler->SetFilteringMode( EngineConstantsMapper::EngineConstant( tfm ) );
}

// **************************
//
void            FullscreenEffectVisualComponent::SyncStaticTexture                   ( unsigned int i, const Texture2DEntry & tex )
{
    SyncTexture( i, tex.GetTexture() );
	SyncSampler( i, tex.GetSamplerName(), tex.GetFilteringMode(), tex.GetWrappingModeX(), tex.GetWrappingModeY() );
}


} //bv
