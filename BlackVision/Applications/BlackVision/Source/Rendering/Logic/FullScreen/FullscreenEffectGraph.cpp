#include "FullscreenEffectGraph.h"

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
void                            FullscreenEffectGraph::InsertNode      ( FullscreenEffectGraphNodePtr node, const std::vector< FullscreenEffectGraphNodePtr > & predcessors )
{
    assert( node->GetInputVec().size() == 0 );

    for( auto predcessor : predcessors )
    {
        assert( m_nodes.find( predcessor ) != m_nodes.end() );

        node->AddInput( predcessor ); 
    }

    assert( m_nodes.find( node ) == m_nodes.end() );
    m_nodes.insert( node );
}

// ****************************
//
void                            FullscreenEffectGraph::SetSinkNode      ( FullscreenEffectGraphNodePtr node )
{
    assert( m_nodes.find( node ) != m_nodes.end() );

    m_sinkNode = node;
}

} //bv
