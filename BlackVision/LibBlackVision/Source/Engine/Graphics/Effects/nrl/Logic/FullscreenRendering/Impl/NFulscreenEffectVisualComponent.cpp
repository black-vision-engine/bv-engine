#include "NFulscreenEffectVisualComponent.h"

#include "Engine/Graphics/SceneGraph/TriangleStrip.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParameters.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Graphics/Shaders/RenderableEffect.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/Impl/NFullscreenVSShader.h"
#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/Impl/NFullscreenEffectHelpers.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv { namespace nrl {

// **************************
//
NFullscreenEffectVisualComponent::NFullscreenEffectVisualComponent    ( TriangleStrip * quad, unsigned int numRendenderTargetTextures, unsigned int numStaticTextures, const IValuePtrVec & values )
    : m_quad( quad )
    , m_numRenderTargetTextures( numRendenderTargetTextures )
    , m_numStaticTextures( numStaticTextures )
    , m_values( values )

{
    auto effect = m_quad->GetRenderableEffect();
    auto pass = effect->GetPass( 0 );

    m_pixelShader   = pass->GetPixelShader();
}

// **************************
//
NFullscreenEffectVisualComponent::~NFullscreenEffectVisualComponent     ()
{
    delete m_quad;
}

// **************************
//
void    NFullscreenEffectVisualComponent::Render                        ( Renderer * renderer )
{
    // At this point all input data must be synchronized.
    // This class is not responsible for tracking external sources of possible state changes.

    renderer->Draw( m_quad );
}

// **************************
//
void    NFullscreenEffectVisualComponent::SyncRenderTargets             ( const RenderTargetVec & renderTargets )
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
void    NFullscreenEffectVisualComponent::SyncTextures                  ( const NTexture2DEntryVec & textures )
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
void    NFullscreenEffectVisualComponent::SyncRenderState               ( const NFullscreenEffectRendererState & state )
{
    auto sinst = GetRenderStateInstance( m_quad );
    
    NFullscreenEffectHelpers::SetRenderState( sinst, state );
}

// **************************
//
void    NFullscreenEffectVisualComponent::SyncIValues                   ( const IValuePtrVec & vals )
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

        numUpdated++;

        param->UpdateValuePtr( val.get() );
    }

    // FIXME: this test does not include repeated values, but at this moment it doesn't seem too important
    assert( numUpdated == shaderParams->NumParameters() );
}

// **************************
//
const IValuePtrVec &    NFullscreenEffectVisualComponent::GetIValues    () const
{
    return m_values;
}

// **************************
//
NFullscreenEffectVisualComponent *   NFullscreenEffectVisualComponent::Create( const NFullscreenEffectVisualComponentDesc & desc )
{
    auto & pixelShaderSource    = desc.GetPixelShaderSrc();
    auto & textures             = desc.GetStaticTextures();
    auto & values               = desc.GetValues();
    auto & rtInputSamplerNames  = desc.GetInputSamplerNames();
    auto & rendererState        = desc.GetRendererState();

    auto geom = NFullscreenEffectHelpers::CreateFullscreenRenderable( pixelShaderSource, textures, values, rtInputSamplerNames, rendererState );

    unsigned int numRenderTargetTextures    = (unsigned int) rtInputSamplerNames.size();
    unsigned int numStaticTextures          = (unsigned int) textures.size();
    
    return new NFullscreenEffectVisualComponent( geom, numRenderTargetTextures, numStaticTextures, values );
}

// **************************
//
unsigned int    NFullscreenEffectVisualComponent::FirstRenderTargetTextureIdx         () const
{
    return 0;
}

// **************************
//
unsigned int    NFullscreenEffectVisualComponent::FirstStaticTextureIdx               () const
{
    return FirstRenderTargetTextureIdx() + m_numRenderTargetTextures;
}

// **************************
//
void            NFullscreenEffectVisualComponent::SyncTexture                         ( unsigned int i, Texture2DPtr tex )
{
    auto params = m_pixelShader->GetParameters();

    params->SetTexture( i, tex );
}

// **************************
//
void            NFullscreenEffectVisualComponent::SyncSampler                         ( unsigned int i, const std::string & name, TextureFilteringMode tfm, TextureWrappingMode wrapX, TextureWrappingMode wrapY )
{
    auto & samplers = m_pixelShader->Samplers();

    auto sampler = samplers[ i ];

    assert( sampler->GetName() == name );

    sampler->SetWrappingMode( EngineConstantsMapper::EngineConstant( wrapX ), SamplerWrapDirection::SWD_S );
    sampler->SetWrappingMode( EngineConstantsMapper::EngineConstant( wrapY ), SamplerWrapDirection::SWD_T );
    sampler->SetFilteringMode( EngineConstantsMapper::EngineConstant( tfm ) );    
}

// **************************
//
void            NFullscreenEffectVisualComponent::SyncStaticTexture                   ( unsigned int i, const NTexture2DEntry & tex )
{
    SyncTexture( i, tex.GetTexture() );
    SyncSampler( i, tex.GetSamplerName(), tex.GetFilteringMode(), tex.GetWrappingModeX(), tex.GetWrappingModeY() );
}

} //nrl
} //bv
