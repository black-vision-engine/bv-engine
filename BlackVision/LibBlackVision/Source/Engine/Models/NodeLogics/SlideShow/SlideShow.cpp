#include "stdafx.h"
#include "SlideShow.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"

#include "System/Time.h"



namespace bv {
namespace nodelogic
{

const std::string       SlideShow::m_type = "SlideShow";

const std::string           SlideShow::ACTION::START_SLIDE_SHOW = "Start";
const std::string           SlideShow::ACTION::STOP_SLIDE_SHOW  = "Stop";
const std::string           SlideShow::ACTION::PAUSE_SLIDE_SHOW = "Pause";

const std::string           SlideShow::PARAMETERS::FADE_TIME        = "FadeTime";
const std::string           SlideShow::PARAMETERS::FADE_TYPE        = "FadeType";
const std::string           SlideShow::PARAMETERS::PRESENCE_TIME    = "PresenceTime";


// ***********************
//
const std::string &     SlideShow::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     SlideShow::GetType             () const
{
    return Type();
}

// ***********************
//
SlideShow::SlideShow             ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator )
    :   m_parentNode( parent )
    ,   m_nodeIdx( 0 )
    ,   m_started( false )
{
    model::ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();
    h.AddEnumParam( PARAMETERS::FADE_TYPE, FadeType::NoFade, false, false );
    h.AddSimpleParam( PARAMETERS::FADE_TIME, 0.3f, true, false );
    h.AddSimpleParam( PARAMETERS::PRESENCE_TIME, 3.0f, true, false );

    m_paramValModel = std::static_pointer_cast<model::DefaultParamValModel>( h.GetModel()->GetPluginModel() );

    m_fadeType = QueryTypedEnum< FadeType >( PARAMETERS::FADE_TYPE );
    m_fadeTime = QueryTypedValue< ValueFloatPtr >( m_paramValModel->GetValue( PARAMETERS::FADE_TIME ) );
    m_presenceTime = QueryTypedValue< ValueFloatPtr >( m_paramValModel->GetValue( PARAMETERS::PRESENCE_TIME ) );
}

// ***********************
//
SlideShow::~SlideShow()
{}


// ***********************
//
void                        SlideShow::Update			( TimeType t )
{
    NodeLogicBase::Update( t );

    if( m_started )
    {
        auto delta = ( Time::Now() - m_startTime ) * 0.001;
        if( delta > m_presenceTime->GetValue() )
        {
            HideAllNodes();
            m_startTime = Time::Now();

            auto parentNode = m_parentNode.lock();

            m_nodeIdx = ++m_nodeIdx % parentNode->GetNumChildren();
            ShowNode( m_nodeIdx );
        }
    }
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        SlideShow::Serialize       ( ISerializer & ser ) const
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
SlideShowPtr              SlideShow::Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< SlideShow >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        SlideShow::HandleEvent     ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    if( action == SlideShow::ACTION::START_SLIDE_SHOW )
    {
        HideAllNodes();

        m_startTime = Time::Now();

        m_nodeIdx = 0;
        ShowNode( m_nodeIdx );

        m_started = true;
        return true;
    }
    else if( action == SlideShow::ACTION::PAUSE_SLIDE_SHOW )
    {

    }
    else if( action == SlideShow::ACTION::STOP_SLIDE_SHOW )
    {

        m_started = false;
        return true;
    }

    return false;
}


// ***********************
//
void        SlideShow::HideAllNodes ()
{
    ShowAllNodes( false );
}

// ***********************
//
void        SlideShow::ShowAllNodes ( bool value )
{
    if( auto parentNode = m_parentNode.lock() )
    {
        for( unsigned int i = 0; i < parentNode->GetNumChildren(); ++i )
        {
            parentNode->GetChild( i )->SetVisible( value );
        }
    }
}

// ***********************
//
void        SlideShow::ShowNode     ( int idx )
{
    if( auto parentNode = m_parentNode.lock() )
    {
        parentNode->GetChild( idx )->SetVisible( true );
    }
}

}   // nodelogic
}	// bv
