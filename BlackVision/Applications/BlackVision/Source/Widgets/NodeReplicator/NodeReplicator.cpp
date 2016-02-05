#include "NodeReplicator.h"

#include "Engine/Models/ModelNodeEditor.h"

#include "ShiftReplicationModifier.h"

namespace bv{ namespace model {

// *******************************
//
NodeReplicator::NodeReplicator( BasicNode * node, SizeType repNum, const IReplicationModifierConstPtr & modifier )
    : m_node( node )
    , m_repModifier( modifier )
    , m_repNum( repNum )
    , m_initialized( false )
{
}

// *******************************
//
NodeReplicatorPtr           NodeReplicator::Create( BasicNode * node, SizeType repNum, const IReplicationModifierConstPtr & modifier )
{
    return NodeReplicatorPtr( new NodeReplicator( node, repNum, modifier ) );
}

// *******************************
//
void					    NodeReplicator::Initialize()
{
    if( m_initialized )
    {
        return;
    }

    m_initialized = true;

    auto numChildren = m_node->GetNumChildren();

    if( numChildren > 0 )
    {
        auto toReplicate = m_node->GetChild( numChildren - 1 );

        auto basicName = toReplicate->GetName();
        
        for( SizeType i = 0; i < m_repNum; ++i )
        {
            auto copiedNode = toReplicate->GetModelNodeEditor()->CopyNode();

            copiedNode->SetName( basicName + "_rep" + std::to_string( i ) );

            m_repModifier->Apply( toReplicate, copiedNode );

            m_node->AddChildToModelOnly( copiedNode );

            toReplicate = copiedNode;
        }   
    }
}

// *******************************
//
void					    NodeReplicator::Update( TimeType )
{}

// *******************************
//
void					    NodeReplicator::Deinitialize()
{}


// ***********************
//
void                NodeReplicator::Serialize       ( ISerializer& ser ) const
{
    ser.EnterChild( "logic" );
        ser.SetAttribute( "type", "replicate" );
        ser.SetAttribute( "numRepetitions", SerializationHelper::T2String( m_repNum ) );

        m_repModifier->Serialize( ser );

    ser.ExitChild();
}

// ***********************
//
NodeReplicatorPtr    NodeReplicator::Create          ( const IDeserializer & deser, bv::model::BasicNode * parentNode )
{
    SizeType repetitions = SerializationHelper::String2T( deser.GetAttribute( "numRepetitions" ), 0 );
    
    deser.EnterChild( "replicatorModifier" );
        std::string modifierType = deser.GetAttribute( "type" );
        
        IReplicationModifierConstPtr modifier;
        if( modifierType == "shiftReplicationModifier" )
        {
            modifier = ShiftReplicationModifier::Create( deser );\
        }
        else
        {
            modifier = nullptr;
        }


    deser.ExitChild();  // replicatorModifier
    
    if( !modifier )
        return nullptr;

    auto replicator = NodeReplicator::Create( parentNode, repetitions, modifier );
    return replicator;
}

// ***********************
//
bool                NodeReplicator::HandleEvent     ( IDeserializer& /*eventSer*/, ISerializer& /*response*/, BVProjectEditor * /*editor*/ )
{
    // @todo Implement all posible interactions with this widget.
    return false;
}

} // model
} // bv