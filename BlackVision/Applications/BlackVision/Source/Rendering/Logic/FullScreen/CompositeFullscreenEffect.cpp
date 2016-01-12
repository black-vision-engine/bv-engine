#include "CompositeFullscreenEffect.h"

#include <cassert>

#include "Rendering/Logic/FullScreen/FullscreenEffectGraph.h"

#include "Rendering/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Rendering/Logic/FullScreen/FullscreenEffectGraph.h"


namespace bv {

// ****************************
//
CompositeFullscreenEffect::CompositeFullscreenEffect   ( FullscreenEffectGraph * graph )
    : m_graph( graph )
{
}

// ****************************
//
CompositeFullscreenEffect::~CompositeFullscreenEffect  ()
{
    delete m_graph;
}

// ****************************
//
void    CompositeFullscreenEffect::Render                      ( FullscreenEffectContext * ctx )
{
    assert( GetNumInputs() <= ( ctx->AccessInputRenderTargets()->size() - ctx->GetFirstRenderTargetIndex() ) );

    SetInputRenderTargets( m_graph->GetSourceNodes(), ctx->AccessInputRenderTargets(), ctx->GetFirstRenderTargetIndex() );

    RenderGraphNode( m_graph->GetSinkNode(), ctx->GetRenderer(), ctx->GetOutputRenderTarget(), ctx->GetRenderTargetAllocator() );
}

// ****************************
//
unsigned int    CompositeFullscreenEffect::GetNumInputs                () const
{
    unsigned int sum = 0;

    for( auto node : m_graph->GetSourceNodes() )
    {
        sum += node->GetNumInputNodes();
    }

    return sum;
}

// ****************************
//
void    CompositeFullscreenEffect::RenderGraphNode             ( FullscreenEffectGraphNodePtr node, Renderer * renderer, RenderTarget * outputRenderTarget, RenderTargetStackAllocator * allocator )
{
    std::vector< RenderTarget * >   inputResults( node->GetNumInputNodes() );

    for( auto it : node->GetInputVec() )
    {
        auto nodeOutRt = allocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
        RenderGraphNode( it, renderer, nodeOutRt, allocator );
        
        inputResults.push_back( nodeOutRt );

        allocator->Free();
    }

    auto effect = node->GetEffect();

    auto ctx = FullscreenEffectContext( renderer, outputRenderTarget, allocator, 0 );
    ctx.SetInputRenderTargets( &inputResults );

    renderer->Enable( outputRenderTarget );
    effect->Render( &ctx );
    renderer->Disable( outputRenderTarget );
}

// ****************************
//
void    CompositeFullscreenEffect::SetInputRenderTargets    ( const std::vector< FullscreenEffectGraphNodePtr > & sourceNodes, const std::vector< RenderTarget * > * inputRenderTargets, unsigned int startIdx ) const
{
    { sourceNodes; inputRenderTargets; startIdx; }
    for( auto node : sourceNodes )
    {
    }
}

// ****************************
//
void    CompositeFullscreenEffect::SynchronizeInputData        ( FullscreenEffectContext * ctx )
{
    { ctx; }
}

} //bv
