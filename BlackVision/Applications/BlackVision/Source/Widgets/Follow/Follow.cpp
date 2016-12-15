#include "stdafxBVApp.h"
#include "Follow.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Widgets/NodeLogicHelper.h"



namespace bv {
namespace nodelogic
{

const std::string       Follow::m_type = "Follow";

//const std::string       Follow::ACTION::ACTION_NAME        = "ActionName";

//const std::string       Follow::PARAMETERS::PARAMETER_NAME = "ParamName";


// ***********************
//
const std::string &     Follow::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     Follow::GetType             () const
{
    return Type();
}

// ***********************
//
Follow::Follow             ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator )
    : m_parentNode( parent )
{}

// ***********************
//
Follow::~Follow()
{}

// ***********************
//
void                        Follow::Update			( TimeType /*t*/ )
{

}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        Follow::Serialize       ( ISerializer & ser ) const
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
FollowPtr              Follow::Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< Follow >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        Follow::HandleEvent     ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    std::string action = eventDeser.GetAttribute( "Action" );

//    if( action == Follow::ACTION::ACTION_NAME )
//    {
//        return false
//    }

    return false;
}

}   // nodelogic
}	// bv
