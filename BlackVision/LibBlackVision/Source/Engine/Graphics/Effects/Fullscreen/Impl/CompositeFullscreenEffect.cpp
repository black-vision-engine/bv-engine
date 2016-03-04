#include "stdafx.h"

#include "CompositeFullscreenEffect.h"

#include <cassert>

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Effects/Utils/RenderTargetStackAllocator.h"

#include "Engine/Graphics/Effects/FullScreen/Impl/Graph/FullscreenEffectGraph.h"
#include "Engine/Graphics/Effects/FullScreen/Impl/Graph/InputFullscreenEffectGraphNode.h"

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
	auto sinkNode = m_graph->GetSinkNode();

	for( auto n : sinkNode->GetInputVec() )
    {
		auto eff = n->GetEffect();
        if( eff != nullptr )
		{
			eff->Update();
		}
    }

	auto eff = sinkNode->GetEffect();
	if( eff != nullptr )
	{
		eff->Update();
	}
}

// ****************************
//
void    CompositeFullscreenEffect::Render                      ( FullscreenEffectContext * ctx )
{
    SynchronizeInputData( ctx );
    RenderGraphNode( m_graph->GetSinkNode(), ctx );
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
void    CompositeFullscreenEffect::SynchronizeInputData        ( FullscreenEffectContext * ctx )
{
    assert( ctx->AccessInputRenderTargets() != nullptr );
    assert( GetNumInputs() <= ( ctx->AccessInputRenderTargets()->size() - ctx->GetFirstRenderTargetIndex() ) );
    
    auto curIdx     = ctx->GetFirstRenderTargetIndex();
    auto rtVec      = *ctx->AccessInputRenderTargets();

    for( auto node : m_graph->GetSourceNodes() )
    {
        auto effect = node->GetEffect();
        if( node->GetNumInputNodes() != 0 )
        {
            for( auto in : node->GetInputVec() )
            {
                assert( in->GetEffect() == nullptr );
            }
        }
        
        ctx->SetFirstRenderTargetIndex( curIdx );
        effect->SynchronizeInputData( ctx );

        curIdx += effect->GetNumInputs();
    }
}

// ****************************
//
void    CompositeFullscreenEffect::RenderGraphNode             ( FullscreenEffectGraphNodePtr node, FullscreenEffectContext * ctx )
{
	auto renderer = ctx->GetRenderer();
	auto outputRenderTarget = ctx->GetOutputRenderTarget();
	auto allocator = ctx->GetRenderTargetAllocator();

    std::vector< RenderTarget * >   inputResults;

    for( auto it : node->GetInputVec() )
    {
        if( it->GetEffect() != nullptr )
        {
            auto nodeOutRt = allocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
			auto inputCtx = FullscreenEffectContext( renderer, nodeOutRt, allocator, 0 );
			inputCtx.SetInputRenderTargets( ctx->AccessInputRenderTargets() );
            RenderGraphNode( it, &inputCtx );
        
            inputResults.push_back( nodeOutRt );

            allocator->Free();
        }
    }

    auto effect = node->GetEffect();

    if( effect != nullptr )
    {
		if( m_graph->IsSourceNode( node ) )
		{
			if( node->GetNumInputNodes() > 0 )
			{
				auto ctxInputRenderTargets = ctx->AccessInputRenderTargets();
				assert( ctxInputRenderTargets->size() == node->GetNumInputNodes() );

				inputResults.insert( inputResults.begin(), ctxInputRenderTargets->begin(), ctxInputRenderTargets->end() );
			}
		}

        auto ctx = FullscreenEffectContext( renderer, outputRenderTarget, allocator, 0 );
        ctx.SetInputRenderTargets( &inputResults );

        renderer->Enable( outputRenderTarget );
        effect->Render( &ctx );
        renderer->Disable( outputRenderTarget );
    }
}

// ****************************
//
std::vector< IValuePtr > CompositeFullscreenEffect::GetValues       () const
{
    auto sinkInputNodes = m_graph->GetSinkNode()->GetInputVec();

    std::set< IValuePtr > valuesSet;

    for( auto n :  sinkInputNodes )
    {
        auto effect = n->GetEffect();

        if( effect != nullptr )
        {
            for( auto v : effect->GetValues() )
            {
                valuesSet.insert( v );
            }
        }
    }


    auto values = m_graph->GetSinkNode()->GetEffect()->GetValues();

    for( auto v : values )
    {
        valuesSet.insert( v );
    }

    return std::vector< IValuePtr >( valuesSet.begin(), valuesSet.end() );
}

// ****************************
//
bool            CompositeFullscreenEffect::AddAdditionalPreLogicInputs ( SizeType numAddFSELoginInputs )
{
    auto sourceNodes = m_graph->GetSourceNodes();

    if( sourceNodes.size() > 0 )
    {
        for( auto sn : m_graph->GetSourceNodes() )
        {
            for( SizeType i = 0; i < numAddFSELoginInputs; ++i )
            {
                sn->AddInput( std::shared_ptr< InputFullscreenEffectGraphNode >( new InputFullscreenEffectGraphNode() ) );
            }
        }
        
        return true;
    }
    else
    {
        return false;
    }
}

} //bv
