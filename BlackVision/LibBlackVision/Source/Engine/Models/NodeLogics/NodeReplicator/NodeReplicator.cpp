#include "stdafx.h"

#include "NodeReplicator.h"

#include "Engine/Models/ModelNodeEditor.h"

#include "ShiftReplicationModifier.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Models/BVProjectEditor.h"
#include "Serialization/BV/BVDeserializeContext.h"


namespace bv { namespace nodelogic {

const std::string   NodeReplicator::m_type = "replicator";

// ***********************
//
const std::string &  NodeReplicator::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string & NodeReplicator::GetType             () const
{
    return Type();
}

// *******************************
//
NodeReplicator::NodeReplicator( model::BasicNodeWeakPtr node, SizeType repNum, const IReplicationModifierPtr & modifier )
    : m_node( node )
    , m_repModifier( modifier )
    , m_repNum( repNum )
    , m_initialized( false )
{
}

// *******************************
//
NodeReplicatorPtr           NodeReplicator::Create( model::BasicNodeWeakPtr node, SizeType repNum, const IReplicationModifierPtr & modifier )
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
}

// *******************************
//
void					    NodeReplicator::Deinitialize()
{}

// ***********************
//
void                NodeReplicator::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast< BVSerializeContext * >( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );
        ser.SetAttribute( "type", m_type );

        if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
        {
            ser.SetAttribute( "numRepetitions", SerializationHelper::T2String( m_repNum ) );
            m_repModifier->Serialize( ser );
        }

    ser.ExitChild();
}

// ***********************
//
NodeReplicatorPtr    NodeReplicator::Create          ( const IDeserializer & deser, model::BasicNodeWeakPtr parentNode )
{
    SizeType repetitions = SerializationHelper::String2T( deser.GetAttribute( "numRepetitions" ), 0 );
    
    deser.EnterChild( "replicatorModifier" );

    std::string modifierType = deser.GetAttribute( "type" );
        
    IReplicationModifierPtr modifier;
    if( modifierType == "shiftReplicationModifier" )
    {
        modifier = ShiftReplicationModifier::Create( deser );
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
model::IParameterPtr                     NodeReplicator::GetParameter        ( const std::string & ) const
{
    return nullptr;
}

// ***********************
//
const std::vector< model::IParameterPtr > & NodeReplicator::GetParameters    () const
{
    static std::vector< model::IParameterPtr > ret;

    return ret;
}

// ***********************
//
bool                NodeReplicator::HandleEvent     ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
{
    if( auto node = m_node.lock() )
    {
        std::string action = eventSer.GetAttribute( "Action" );

        if( action == "Replicate" )
        {
            auto numChildren = node->GetNumChildren();

            if( numChildren > 0 )
            {
                auto toReplicate = node->GetChild( numChildren - 1 );

                auto basicName = toReplicate->GetName();

                auto context = static_cast< BVDeserializeContext * >( eventSer.GetDeserializeContext() );
                auto scene = editor->GetModelScene( context->GetSceneName() );

                for( SizeType i = 0; i < m_repNum; ++i )
                {
                    auto copiedNode = editor->AddNodeCopy( scene, node, scene, toReplicate );

                    editor->RenameNode( copiedNode, basicName + "_rep" + std::to_string( i ) );

                    m_repModifier->Apply( toReplicate, copiedNode, editor, ( int )i );

                    toReplicate = copiedNode;
                }

                response.SetAttribute( COMMAND_SUCCESS_STRING, "Node replicated." );
                return true;
            }
            else
            {
                response.SetAttribute( ERROR_INFO_STRING, "Node has no child. Cannot replicate" );
            }
        }
        if( action == "GetModifier" )
        {
            m_repModifier->Serialize( response );
            return true;
        }
        else if( action == "GetNumRepetitions" )
        {
            response.SetAttribute( "numRepetitions", SerializationHelper::T2String( m_repNum ) );
            return true;
        }
        else if( action == "SetNumRepetitions" )
        {
            m_repNum = SerializationHelper::String2T< int >( eventSer.GetAttribute( "numRepetitions" ), 0 );
            return true;
        }
        else if( action == "AddParamShift" )
        {
            return AddParamShift( eventSer, response, editor );
        }
        else if( action == "RemoveParamShift" )
        {
            return RemoveParamShift( eventSer, response, editor );
        }
        else if( action == "ClearShifts" )
        {
            return ClearShifts( eventSer, response, editor );
        }
        else
        {
            response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only 'Replicate' command." );
        }
    }

    return false;
}

// ***********************
//
bool            NodeReplicator::AddParamShift       ( IDeserializer & eventSer, ISerializer & /*response*/, BVProjectEditor * /*editor */)
{
    auto modifier = std::static_pointer_cast< ShiftReplicationModifier >( m_repModifier );
    return modifier->AddParamShift( eventSer, modifier );
}

// ***********************
//
bool            NodeReplicator::RemoveParamShift    ( IDeserializer & eventSer, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    auto modifier = std::static_pointer_cast< ShiftReplicationModifier >( m_repModifier );

    std::string pluginName = eventSer.GetAttribute( "pluginName" );
    std::string paramName = eventSer.GetAttribute( "paramName" );
    TimeType startTime = SerializationHelper::String2T( eventSer.GetAttribute( "startTime" ), 0.0f );

    return modifier->RemoveParamShift( pluginName, paramName, startTime );
}

// ***********************
//
bool            NodeReplicator::ClearShifts         ( IDeserializer & /*eventSer*/, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    auto modifier = std::static_pointer_cast< ShiftReplicationModifier >( m_repModifier );
    modifier->ClearAllShifts();

    return true;
}

} // nodelogic
} // bv