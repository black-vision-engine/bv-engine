#include "stdafx.h"

#include "FullscreenEffectGraph.h"

#include "Engine/Graphics/Effects/Fullscreen/Impl/CompositeFullscreenEffect.h"

#include <cassert>


namespace  bv {

// ****************************
//
FullscreenEffectGraph::FullscreenEffectGraph   ()
    : m_sinkNode( nullptr )
{
}

// ****************************
//
FullscreenEffectGraph::~FullscreenEffectGraph  ()
{
}

// ****************************
//
FullscreenEffectGraphNodePtr    FullscreenEffectGraph::GetSinkNode     ()
{
    return m_sinkNode;
}

// ****************************
//
void                            FullscreenEffectGraph::InsertNode      ( FullscreenEffectGraphNodePtr node, const std::vector< FullscreenEffectGraphNodePtr > & predecessors )
{
    assert( node->GetNumInputNodes() == 0 );

    for( auto predcessor : predecessors )
    {
        assert( m_nodes.find( predcessor ) != m_nodes.end() );

        node->AddInput( predcessor ); 
    }

    assert( m_nodes.find( node ) == m_nodes.end() );
    m_nodes.insert( node );
}

// ****************************
//
void                            FullscreenEffectGraph::AddNode         ( FullscreenEffectGraphNodePtr node )
{
    m_nodes.insert( node );

    for( auto in : node->GetInputVec() )
    {
        AddNode( in );
    }
}

// ****************************
//
void                            FullscreenEffectGraph::SetSinkNode      ( FullscreenEffectGraphNodePtr node )
{
    assert( m_nodes.find( node ) != m_nodes.end() );

    m_sinkNode = node;
}

// ****************************
//
void                            FullscreenEffectGraph::MarkSourceNode          ( FullscreenEffectGraphNodePtr node )
{
    assert( m_nodes.find( node ) != m_nodes.end() );
    assert( node->GetNumInputNodes() == 0 );

    assert( std::dynamic_pointer_cast< InputFullscreenEffectGraphNode >( node ) );

    m_sourceNodes.push_back( node );
}

// ****************************
//
const std::vector< FullscreenEffectGraphNodePtr > &     FullscreenEffectGraph::GetSourceNodes  () const
{
    return m_sourceNodes;
}

// ****************************
//
std::vector< FullscreenEffectGraphNodePtr >  &          FullscreenEffectGraph::GetSourceNodes  ()
{
    return m_sourceNodes;
}

// ****************************
//
bool							FullscreenEffectGraph::IsSourceNode ( FullscreenEffectGraphNodePtr node ) const
{
	for( auto sn : m_sourceNodes )
	{
		if( node == sn )
		{
			return true;
		}
	}
	return false;
}

// ****************************
//
SizeType                        FullscreenEffectGraph::SourceNodeIndex( FullscreenEffectGraphNodePtr node ) const
{
    assert( IsSourceNode( node ) );

    for( SizeType i = 0; i < m_sourceNodes.size(); ++i )
	{
		if( node == m_sourceNodes[ i ] )
		{
            return i;
		}
	}

    return 0;
}

// ****************************
//
void                            FullscreenEffectGraph::FlattenGraph ()
{
    m_sinkNode = FlattenNode( m_sinkNode );

    m_nodes.clear();
    AddNode( m_sinkNode );
}

// ****************************
//
FullscreenEffectGraphNodePtr        FullscreenEffectGraph::FlattenNode   ( FullscreenEffectGraphNodePtr node )
{
    if( auto composite = dynamic_cast< CompositeFullscreenEffect * >( node->GetEffect() ) )
    {
        auto graph = composite->GetGraph();
        graph->FlattenGraph();

        auto sinkNode = graph->GetSinkNode();

        auto sourceNodes = composite->GetGraph()->GetSourceNodes();
        auto inputs = node->GetInputVec();

        assert( inputs.size() == sourceNodes.size() );

        for( auto sn : sourceNodes ) 
        {
            for( auto in : inputs ) 
            {
                sinkNode->SwapInput( sn, in );
            }
        }

        return composite->GetGraph()->GetSinkNode();
    }
    else
    {
        for( auto & in : node->GetInputVec() )
        {
            in = FlattenNode( in );
        }
        return node;
    }
}

} //bv
