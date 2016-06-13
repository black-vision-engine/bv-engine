#include "Arrange.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Widgets/NodeLogicHelper.h"



namespace bv { namespace nodelogic
{

const std::string       Arrange::m_type = "Arrange";

//const std::string       Arrange::ACTION::ACTION_NAME        = "ActionName";

//const std::string       Arrange::PARAMETERS::PARAMETER_NAME = "ParamName";


// ***********************
//
const std::string &     Arrange::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     Arrange::GetType             () const
{
    return Type();
}

// ***********************
//
Arrange::Arrange             ( bv::model::BasicNodePtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator )
    :   m_parentNode( parent )
{}

// ***********************
//
Arrange::~Arrange()
{}

// ***********************
//
void                        Arrange::Update			( TimeType /*t*/ )
{

}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        Arrange::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
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
ArrangePtr              Arrange::Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< Arrange >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        Arrange::HandleEvent     ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    //    if( action == Arrange::ACTION::ACTION_NAME )
    //    {
    //        return false
    //    }

    return false;
}

}   // nodelogic
}	// bv
