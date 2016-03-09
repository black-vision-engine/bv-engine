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
	Update( m_graph->GetSinkNode() );
}

// ****************************
//
void    CompositeFullscreenEffect::Update                       ( FullscreenEffectGraphNodePtr node ) 
{
    for( auto inputNode : node->GetInputVec() )
    {
        Update( inputNode );
    }
    
    auto eff = node->GetEffect();
    
    if( eff != nullptr )
	{
		eff->Update();
	}
}

// ****************************
//
void    CompositeFullscreenEffect::Render                       ( FullscreenEffectContext * ctx )
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
        differentInputs.insert( node );
    }

    return ( unsigned int ) differentInputs.size();
}

// ****************************
//
void    CompositeFullscreenEffect::SynchronizeInputData        ( FullscreenEffectContext * ctx )
{
    assert( ctx->AccessInputRenderTargets() != nullptr );
    assert( m_graph->GetSourceNodes().size() == ctx->AccessInputRenderTargets()->size() );

    auto inputRTs = ctx->AccessInputRenderTargets();

    m_sourceRenderTargets.clear();

    m_sourceRenderTargets.insert( m_sourceRenderTargets.begin(), inputRTs->begin(), inputRTs->end() );

    assert( m_sourceRenderTargets.size() == inputRTs->size() );
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
        if( !m_graph->IsSourceNode( it ) ) // Effect is null when the node is a special node which means that it's an input from PreLogic.
        {
            auto nodeOutRt = allocator->Allocate( RenderTarget::RTSemantic::S_DRAW_ONLY );
			auto inputCtx = FullscreenEffectContext( renderer, nodeOutRt, allocator, 0 );

            RenderGraphNode( it, &inputCtx );
        
            inputResults.push_back( nodeOutRt );

            allocator->Free();
        }
        else
        {
            auto idx = m_graph->SourceNodeIndex( it );

            assert( idx < m_sourceRenderTargets.size() );

            inputResults.push_back( m_sourceRenderTargets[ idx ] );
        }
    }

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
