#include "NodeReplicator.h"

#include "Engine/Models/ModelNodeEditor.h"

namespace bv{ namespace model {

// *******************************
//
NodeReplicator::NodeReplicator( const BasicNodePtr & node, SizeType repNum, const IReplicationModifierConstPtr & modifier )
    : m_node( node )
    , m_repModifier( modifier )
    , m_repNum( repNum )
{}

// *******************************
//
NodeReplicatorConstPtr      NodeReplicator::Create( const BasicNodePtr & node, SizeType repNum, const IReplicationModifierConstPtr & modifier )
{
    return NodeReplicatorConstPtr( new NodeReplicator( node, repNum, modifier ) );
}

// *******************************
//
void					    NodeReplicator::Initialize()
{
    auto numChildren = m_node->GetNumChildren();

    if( numChildren > 0 )
    {
        auto toReplicate = m_node->GetChild( numChildren - 1 );
        
        for( SizeType i = 0; i < m_repNum; ++i )
        {
            auto copiedNode = toReplicate->GetModelNodeEditor()->CopyNode();

            m_repModifier->Apply( toReplicate, copiedNode );

            m_node->AddChildToModelOnly( copiedNode );

            toReplicate = copiedNode;
        }
        
    }

    
}

// *******************************
//
void					    NodeReplicator::Update( TimeType )
{
}

// *******************************
//
void					    NodeReplicator::Deinitialize()
{
}

} // model
} // bv