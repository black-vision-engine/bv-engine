#include "stdafx.h"

#include "FullscreenEffect.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Effects/Logic/State/RenderedData.h"
#include "Engine/Graphics/Effects/Logic/FullscreenRendering/FullscreenEffectInstance/FullscreenEffectInstance.h"

#include "Engine/Graphics/Effects/Logic/FullscreenRendering/Impl/FullscreenEffectHelpers.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"


namespace bv { 

// **************************
//
FullscreenEffect::FullscreenEffect        ( FullscreenEffectInstance * fseInst, FullscreenEffectComponentStatePtr state )
    : m_fseInst( fseInst )
    , m_rendererCamera( nullptr )
    , m_state( state )
{
}

// **************************
//
FullscreenEffect::~FullscreenEffect       ()
{
    delete m_fseInst;
}

// **************************
//
void    FullscreenEffect::Render           ( RenderContext * ctx )
{
    SetFullscreenCamera( renderer( ctx ) );

    m_fseInst->SyncState( m_state );
    m_fseInst->Render( ctx );

    SetRendererCamera( renderer( ctx ) );
}

// **************************
//
void    FullscreenEffect::Render           ( RenderContext * ctx, const RenderedData & dynamicInput )
{
    SetFullscreenCamera( renderer( ctx ) );

    m_fseInst->SyncState( m_state );
    m_fseInst->Render( ctx, dynamicInput );

    SetRendererCamera( renderer( ctx ) );
}

// **************************
//
void    FullscreenEffect::Render           ( RenderContext * ctx, const RenderTarget * output )
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
void    FullscreenEffect::Render           ( RenderContext * ctx, const RenderTarget * output, const RenderedData & dynamicInput )
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
FullscreenEffectComponentStatePtr FullscreenEffect::GetState      ()
{
    return m_state;
}

// **************************
//
void    FullscreenEffect::AppendRenderPasses_DIRTY_HACK   ( std::set< const RenderablePass * > * passes ) const
{
    m_fseInst->AppendRenderPasses_DIRTY_HACK( passes );
}

// **************************
//
void    FullscreenEffect::SetFullscreenCamera	( Renderer * renderer )
{
    assert( m_rendererCamera == nullptr );

    m_rendererCamera = renderer->GetCamera();

    renderer->SetCamera( FullscreenEffectHelpers::DisplayCamera() );
}

// **************************
//
void    FullscreenEffect::SetRendererCamera   ( Renderer * renderer )
{
    // FIXME: nrl 
    //assert( m_rendererCamera != nullptr );

    renderer->SetCamera( m_rendererCamera );

    m_rendererCamera =  nullptr;
}


} //bv
