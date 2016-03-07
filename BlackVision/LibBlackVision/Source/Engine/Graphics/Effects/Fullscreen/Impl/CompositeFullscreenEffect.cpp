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
    std::set< FullscreenEffectGraphNodePtr > differentInputs;

    for( auto node : m_graph->GetSourceNodes() )
    {
        auto nodeInputs = node->GetInputVec();
        differentInputs.insert( nodeInputs.begin(), nodeInputs.end() );
    }

    auto it = differentInputs.begin();

    return ( unsigned int ) differentInputs.size();
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
        assert( node->GetEffect() == nullptr );
        
        ctx->SetFirstRenderTargetIndex( curIdx );
        curIdx++;

        //effect->SynchronizeInputData( ctx );

        //curIdx += effect->GetNumInputs();
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

    for( auto it : node->GetInputVec() ) // Render parents' nodes at first
    {
        if( it->GetEffect() != nullptr ) // Effect is null when the node is a special node which means that it's an input from PreLogic.
        {
            auto nodeOutRt = allocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
			auto inputCtx = FullscreenEffectContext( renderer, nodeOutRt, allocator, 0 );
			inputCtx.SetInputRenderTargets( ctx->AccessInputRenderTargets() );
            RenderGraphNode( it, &inputCtx );
        
            inputResults.push_back( nodeOutRt );

            allocator->Free();
        }
    }

    auto preInputs = ctx->AccessInputRenderTargets();

    if( preInputs != nullptr )
    {
        assert( preInputs->size() == m_graph->GetSourceNodes().size() );
    }

    std::vector< RenderTarget * > preInputResults;

    for( auto inputNode : node->GetInputVec() )
    {
        if( m_graph->IsSourceNode( inputNode ) )
        {
            auto idx = m_graph->SourceNodeIndex( inputNode );
            preInputResults.push_back( ( *preInputs )[ idx ] );
        }
    }

    inputResults.insert( inputResults.begin(), preInputResults.begin(), preInputResults.end() );

    auto effect = node->GetEffect();

    if( effect != nullptr ) // Don't render special node which is input form PreLogic (PreLogic output)
    {
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
bool            CompositeFullscreenEffect::AddAdditionalPreLogicInputs ( const std::vector< InputFullscreenEffectGraphNodePtr > & additionalNodes )
{
    auto sourceNodes = m_graph->GetSourceNodes();

    if( sourceNodes.size() > 0 )
    {
        for( auto sn : m_graph->GetSourceNodes() )
        {
            for( auto n : additionalNodes )
            {
                sn->AddInput( n );
            }
        }
        
        return true;
    }
    else
    {
        return false;
    }
}

// ****************************
//
FullscreenEffectGraph * CompositeFullscreenEffect::GetGraph             ()
{
    return m_graph;
}

} //bv
