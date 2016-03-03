#include "stdafx.h"

#include "CompositeFullscreenEffect.h"

#include <cassert>

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Effects/FullScreen/Impl/Graph/FullscreenEffectGraph.h"

#include <set>

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
void    CompositeFullscreenEffect::Update                      ()
{
    for( auto n : m_graph->GetSourceNodes() )
    {
        n->GetEffect()->Update();
    }
}

// ****************************
//
void    CompositeFullscreenEffect::Render                      ( FullscreenEffectContext * ctx )
{
    SynchronizeInputData( ctx );

    RenderGraphNode( m_graph->GetSinkNode(), ctx->GetRenderer(), ctx->GetOutputRenderTarget(), ctx->GetRenderTargetAllocator() );
}

// ****************************
//
unsigned int    CompositeFullscreenEffect::GetNumInputs                () const
{
    unsigned int sum = 0;

    for( auto node : m_graph->GetSourceNodes() )
    {
        sum += node->GetEffect()->GetNumInputs();
    }

    return sum;
}

// ****************************
//
void    CompositeFullscreenEffect::SynchronizeInputData        ( FullscreenEffectContext * ctx )
{
    assert( ctx->AccessInputRenderTargets() != nullptr );
    assert( GetNumInputs() <= ( ctx->AccessInputRenderTargets()->size() - ctx->GetFirstRenderTargetIndex() ) );
    
    auto curIdx     = ctx->GetFirstRenderTargetIndex();
    auto rtVec      = *ctx->AccessInputRenderTargets();

    for( auto node : m_graph->GetSourceNodes() )
    {
        assert( node->GetNumInputNodes() == 0 );

        auto effect = node->GetEffect();
        
        ctx->SetFirstRenderTargetIndex( curIdx );
        effect->SynchronizeInputData( ctx );

        curIdx += effect->GetNumInputs();
    }
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
std::vector< IValuePtr > CompositeFullscreenEffect::GetValues       () const
{
    auto sinkInputNodes = m_graph->GetSinkNode()->GetInputVec();

    std::set< IValuePtr > valuesSet;

    for( auto n :  sinkInputNodes )
    {
        for( auto v : n->GetEffect()->GetValues() )
        {
            valuesSet.insert( v );
        }
    }


    auto values = m_graph->GetSinkNode()->GetEffect()->GetValues();

    for( auto v : values )
    {
        valuesSet.insert( v );
    }

    return std::vector< IValuePtr >( valuesSet.begin(), valuesSet.end() );
}

} //bv
