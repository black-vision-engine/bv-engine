#include "CompositeFullscreenEffect.h"

#include "Rendering/Logic/FullScreen/FullscreenEffectGraph.h"

#include "Rendering/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Renderers/Renderer.h"


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
    auto renderer = ctx->GetRenderer();

    renderer->Enable( ctx->GetOutputRenderTarget() );


    renderer->Disable( ctx->GetOutputRenderTarget() );
}

// ****************************
//
void    CompositeFullscreenEffect::RenderGraphNode             ( FullscreenEffectGraphNodePtr node, Renderer * renderer, RenderTarget * outputRenderTarget, RenderTargetStackAllocator * allocator )
{
    std::vector< RenderTarget * >   inputResults( node->GetInputVec().size() );

    for( auto it : node->GetInputVec() )
    {
        auto nodeOutRt = allocator->Allocate(RenderTarget::RTSemantic::S_DRAW_ONLY );
        RenderGraphNode( it, renderer, nodeOutRt, allocator );
        
        inputResults.push_back( nodeOutRt );

        allocator->Free();
    }

    auto effect = node->GetEffect();

    auto ctx = FullscreenEffectContext( renderer, outputRenderTarget, allocator );
    ctx.AccessInputRenderTargets() = inputResults;

    renderer->Enable( outputRenderTarget );
    effect->Render( &ctx );
    renderer->Disable( outputRenderTarget );
}

} //bv

