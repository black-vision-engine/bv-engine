#include "stdafxBVApp.h"
#include "Follow.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Widgets/NodeLogicHelper.h"
#include "Engine/Events/InnerEvents/NodeRemovedEvent.h"


DEFINE_ENUM_PARAMETER_FUNCTIONS( bv::nodelogic::Follow::FollowingMode );
DEFINE_ENUM_PARAMETER_FUNCTIONS( bv::nodelogic::Follow::BBAlignementX );
DEFINE_ENUM_PARAMETER_FUNCTIONS( bv::nodelogic::Follow::BBAlignementY );
DEFINE_ENUM_PARAMETER_FUNCTIONS( bv::nodelogic::Follow::BBAlignementZ );





namespace bv {
namespace nodelogic
{



const std::string       Follow::m_type = "Follow";

//const std::string       Follow::ACTION::ACTION_NAME        = "ActionName";

const std::string       Follow::PARAMETERS::OFFSET_X = "OffsetX";
const std::string       Follow::PARAMETERS::OFFSET_Y = "OffsetY";
const std::string       Follow::PARAMETERS::OFFSET_Z = "OffsetZ";

const std::string       Follow::PARAMETERS::ALIGN_X = "AlignX";
const std::string       Follow::PARAMETERS::ALIGN_Y = "AlignY";
const std::string       Follow::PARAMETERS::ALIGN_Z = "AlignZ";

const std::string       Follow::PARAMETERS::FOLLOWING_MODE = "FollowingMode";



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
{
    model::ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleParam( PARAMETERS::OFFSET_X, 0.0f, true, false );
    h.AddSimpleParam( PARAMETERS::OFFSET_Y, 0.0f, true, false );
    h.AddSimpleParam( PARAMETERS::OFFSET_Z, 0.0f, true, false );


    m_paramValModel = std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() );
}

// ***********************
//
Follow::~Follow()
{}

// ***********************
//
void        Follow::Initialize        ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &Follow::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void        Follow::Deinitialize      ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &Follow::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void                        Follow::Update			( TimeType /*t*/ )
{
    //auto node = GetObservedNode();
    //glm::vec3 followedPoint = GetBoxPoint( node, ... );
    //glm::mat4 transformBox = GetBoxTransform( node, ... );
    //glm::vec3 followedPoint = transformBox * followedPoint;
    //ApplyTransform( followedPoint );
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

// ========================================================================= //
// Handling removing of nodes
// ========================================================================= //

// ***********************
//
void                        Follow::NodeRemovedHandler  ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeRemovedEvent::Type() )
        return;

    if( auto parentNode = m_parentNode.lock() )
    {

    }
}


}   // nodelogic
}	// bv
