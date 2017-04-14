#include "stdafx.h"
#include "Cloner.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"



namespace bv
{
namespace nodelogic
{

const std::string       Cloner::m_type = "Cloner";

//const std::string       Cloner::ACTION::ACTION_NAME        = "ActionName";

//const std::string       Cloner::PARAMETERS::PARAMETER_NAME = "ParamName";


// ***********************
//
const std::string &     Cloner::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     Cloner::GetType             () const
{
    return Type();
}

// ***********************
//
Cloner::Cloner             ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator )
    : m_parentNode( parent )
{
    model::ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    m_paramValModel = std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() );
}

// ***********************
//
Cloner::~Cloner()
{}


// ***********************
//
void        Cloner::Initialize        ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &Cloner::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void        Cloner::Deinitialize      ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &Cloner::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void                        Cloner::Update			( TimeType t )
{
    NodeLogicBase::Update( t );
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        Cloner::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast< BVSerializeContext* >( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );
    ser.SetAttribute( "type", m_type );

    if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
    {
        NodeLogicBase::Serialize( ser );
    }

    ser.ExitChild();    // logic
}

// ***********************
//
ClonerPtr              Cloner::Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< Cloner >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        Cloner::HandleEvent     ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    //    if( action == Cloner::ACTION::ACTION_NAME )
    //    {
    //        return false
    //    }

    return false;
}

// ========================================================================= //
// Handling removing of nodes
// ========================================================================= //

// ***********************
//
void                        Cloner::NodeRemovedHandler  ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeRemovedEvent::Type() )
        return;

    if( auto parentNode = m_parentNode.lock() )
    {

    }
}

}   // nodelogic
}	// bv
