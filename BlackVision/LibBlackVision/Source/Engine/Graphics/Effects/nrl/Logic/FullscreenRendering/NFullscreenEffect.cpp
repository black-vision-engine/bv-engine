#include "stdafx.h"

#include "NFullscreenEffect.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderedData.h"
#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/FullscreenEffectInstance/NFullscreenEffectInstance.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/Impl/NFullscreenEffectHelpers.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"


namespace bv { namespace nrl {

// **************************
//
NFullscreenEffect::NFullscreenEffect        ( NFullscreenEffectInstance * fseInst, NFullscreenEffectComponentStatePtr state )
    : m_fseInst( fseInst )
    , m_rendererCamera( nullptr )
    , m_state( state )
{
}

// **************************
//
NFullscreenEffect::~NFullscreenEffect       ()
{
    delete m_fseInst;
}

// **************************
//
void    NFullscreenEffect::Render           ( NRenderContext * ctx )
{
    SetFullscreenCamera( renderer( ctx ) );

    m_fseInst->SyncState( m_state );
    m_fseInst->Render( ctx );

    SetRendererCamera( renderer( ctx ) );
}

// **************************
//
void    NFullscreenEffect::Render           ( NRenderContext * ctx, const NRenderedData & dynamicInput )
{
    SetFullscreenCamera( renderer( ctx ) );

    m_fseInst->SyncState( m_state );
    m_fseInst->Render( ctx, dynamicInput );

    SetRendererCamera( renderer( ctx ) );
}

// **************************
//
void    NFullscreenEffect::Render           ( NRenderContext * ctx, const RenderTarget * output )
{
    assert( output != nullptr );

    auto rendererInst = renderer( ctx );

    SetFullscreenCamera( rendererInst );
    
    const RenderTarget * prevRT = nullptr;

    if( output != ctx->GetBoundRenderTarget() )
    {
        prevRT = ctx->DisableBoundRT();
        ctx->Enable( output );
    }

    m_fseInst->SyncState( m_state );
    m_fseInst->Render( ctx );

    ctx->Disable( output );
    
    if( prevRT != nullptr )
    {
        ctx->Enable( prevRT );
    }

    SetRendererCamera( rendererInst );
}

// **************************
//
void    NFullscreenEffect::Render           ( NRenderContext * ctx, const RenderTarget * output, const NRenderedData & dynamicInput )
{
    assert( output != nullptr );

    auto rendererInst = renderer( ctx );

    SetFullscreenCamera( rendererInst );

    const RenderTarget * prevRT = nullptr;

    if( output != ctx->GetBoundRenderTarget() )
    {
        prevRT = ctx->DisableBoundRT();
        ctx->Enable( output );
    }

    m_fseInst->SyncState( m_state );
    m_fseInst->Render( ctx, dynamicInput );

    ctx->Disable( output );
    
    if( prevRT != nullptr )
    {
        ctx->Enable( prevRT );
    }

    SetRendererCamera( rendererInst );
}

// **************************
//
NFullscreenEffectComponentStatePtr NFullscreenEffect::GetState      ()
{
    return m_state;
}

// **************************
//
void    NFullscreenEffect::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_fseInst->AppendRenderPasses_DIRTY_HACK( passes );
}

// **************************
//
void    NFullscreenEffect::SetFullscreenCamera	( Renderer * renderer )
{
    assert( m_rendererCamera == nullptr );

    m_rendererCamera = renderer->GetCamera();

    renderer->SetCamera( NFullscreenEffectHelpers::DisplayCamera() );
}

// **************************
//
void    NFullscreenEffect::SetRendererCamera   ( Renderer * renderer )
{
    // FIXME: nrl 
    //assert( m_rendererCamera != nullptr );

    renderer->SetCamera( m_rendererCamera );

    m_rendererCamera =  nullptr;
}

} //nrl
} //bv
