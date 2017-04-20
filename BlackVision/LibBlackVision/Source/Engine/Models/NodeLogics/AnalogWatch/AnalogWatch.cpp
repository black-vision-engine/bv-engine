#include "stdafx.h"

#include "AnalogWatch.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"

#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/InnerEvents/Nodes/NodeMovedEvent.h"


// warning C4996: 'localtime': This function or variable may be unsafe
#pragma warning(disable : 4996)
#include <chrono>
#include <ctime>


namespace bv { namespace nodelogic
{

const std::string       AnalogWatch::m_type = "AnalogWatch";

const std::string       AnalogWatch::ACTION::START_WATCH        = "StartWatch";
const std::string       AnalogWatch::ACTION::CLEAR_WATCH        = "ClearWatch";

const std::string       AnalogWatch::PARAMETERS::SMOOTH_HOURS   = "SmoothHours";
const std::string       AnalogWatch::PARAMETERS::SMOOTH_MINUTES = "SmoothMinutes";
const std::string       AnalogWatch::PARAMETERS::SMOOTH_SECONDS = "SmoothSeconds";



// ***********************
//
const std::string &     AnalogWatch::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     AnalogWatch::GetType             () const
{
    return Type();
}

// ***********************
//
AnalogWatch::AnalogWatch             ( model::BasicNodeWeakPtr parent, model::ITimeEvaluatorPtr timeEvaluator )
    : m_parentNode( parent )
    ,   m_hourNode( nullptr )
    ,   m_minuteNode( nullptr )
    ,   m_secondsNode( nullptr )
{
    m_smoothHours = AddBoolParam( m_paramValModel, timeEvaluator, PARAMETERS::SMOOTH_HOURS, false )->Value();
    m_smoothMinutes = AddBoolParam( m_paramValModel, timeEvaluator, PARAMETERS::SMOOTH_MINUTES, false )->Value();
    m_smoothSeconds = AddBoolParam( m_paramValModel, timeEvaluator, PARAMETERS::SMOOTH_SECONDS, false )->Value();
}

// ***********************
//
AnalogWatch::~AnalogWatch()
{}

// ***********************
//
void        AnalogWatch::Initialize        ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &AnalogWatch::NodeRemovedHandler ), NodeRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &AnalogWatch::NodeMovedHandler ), NodeMovedEvent::Type() );
}

// ***********************
//
void        AnalogWatch::Deinitialize      ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &AnalogWatch::NodeRemovedHandler ), NodeRemovedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &AnalogWatch::NodeMovedHandler ), NodeMovedEvent::Type() );
}

// ***********************
//
void                        AnalogWatch::Update			( TimeType t )
{
    if( m_started )
    {
        NodeLogicBase::Update( t );

        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t( now );
    
        tm* localTime = localtime( &time );
        float millis = 0.0f;

        if( m_smoothHours->GetValue() || m_smoothSeconds->GetValue() )
        {
            time_t roundedTime = mktime( localTime );
            auto roundedNow = std::chrono::system_clock::from_time_t( roundedTime );

            auto millisDuration = now - roundedNow;
            millis = (float)std::chrono::duration_cast<std::chrono::milliseconds>( millisDuration ).count();
        }


        float hoursRatio = (float)localTime->tm_hour / 12.0f;
        if( m_smoothHours->GetValue() )
            hoursRatio += (float)localTime->tm_min / ( 60.0f * 12.0f ) + (float)localTime->tm_sec / ( 60.0f * 12.0f * 60.0f );
            

        float minutesRatio = (float)localTime->tm_min / 60.0f;
        if( m_smoothMinutes->GetValue() )
            minutesRatio += (float)localTime->tm_sec / ( 60.0f * 60.0f ) + millis / ( 1000.0f * 60.0f * 60.0f );
            

        float secondsRatio = (float)localTime->tm_sec / 60.0f;
        if( m_smoothSeconds->GetValue() )
            secondsRatio += millis / ( 1000.0f * 60.0f );
    

        UpdateTime( m_hourNode, hoursRatio );
        UpdateTime( m_minuteNode, minutesRatio );
        UpdateTime( m_secondsNode, secondsRatio );
    }
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        AnalogWatch::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast< BVSerializeContext * >( ser.GetSerializeContext() );
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
AnalogWatchPtr              AnalogWatch::Create          ( const IDeserializer & deser, model::BasicNodeWeakPtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto analogWatch = std::make_shared< AnalogWatch >( parentNode, timeline );

    analogWatch->Deserialize( deser );

    return analogWatch;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        AnalogWatch::HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    if( action == AnalogWatch::ACTION::START_WATCH )
    {
        return StartWatch( eventDeser, response, editor );
    }
    else if( action == AnalogWatch::ACTION::CLEAR_WATCH )
    {
        return ClearWatch( eventDeser, response, editor );
    }

    return false;
}


// ***********************
//
bool        AnalogWatch::StartWatch      ( IDeserializer & /*eventSer*/, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    if( auto parentNode = m_parentNode.lock() )
    {
        int numChildren = parentNode->GetNumChildren();

        m_hourNode = numChildren > 1 ? parentNode->GetChild( 0 ).get() : nullptr;
        m_minuteNode = numChildren > 2 ? parentNode->GetChild( 1 ).get() : nullptr;
        m_secondsNode = numChildren > 3 ? parentNode->GetChild( 2 ).get() : nullptr;

        SetInitialPosition( m_hourNode );
        SetInitialPosition( m_minuteNode );
        SetInitialPosition( m_secondsNode );

        m_started = true;

        return true;
    }

    return false;
}

// ***********************
//
bool        AnalogWatch::ClearWatch      ( IDeserializer & /*eventSer*/, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    ClearPosition( m_hourNode );
    ClearPosition( m_minuteNode );
    ClearPosition( m_secondsNode );

    m_hourNode = nullptr;
    m_minuteNode = nullptr;
    m_secondsNode = nullptr;

    m_started = false;

    return true;
}

// ========================================================================= //
// Nodes functions
// ========================================================================= //

// ***********************
//
void        AnalogWatch::SetInitialPosition      ( model::BasicNode * node )
{
    if( node )
    {
        auto boundingVolume = node->GetBoundingVolume();
        if( boundingVolume )
        {
            auto boundingBox = boundingVolume->GetBoundingBox();
            if( boundingBox )
            {
                auto transformParam = node->GetFinalizePlugin()->GetParamTransform();
                
                glm::vec3 center = transformParam->GetTransform().GetCenter( TimeType( 0.0f ) );
                center.y = boundingBox->ymin;

                transformParam->SetCenter( center, TimeType( 0.0f ) );
                transformParam->SetTranslation( glm::vec3( 0.0f, -boundingBox->ymin, 0.0f ), TimeType( 0.0f ) );
            }
        }
    }
}

// ***********************
//
void        AnalogWatch::UpdateTime             ( model::BasicNode * node, float ratio )
{
    if( node )
    {
        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();
        transformParam->SetRotation( glm::vec3( 0.0f, 0.0f, -360.0f * ratio ), TimeType( 0.0f ) );
    }
}

// ***********************
//
void        AnalogWatch::ClearPosition           ( model::BasicNode * node )
{
    if( node )
    {
        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();
        transformParam->SetRotation( glm::vec3( 0.0f, 0.0f, 0.0f ), TimeType( 0.0f ) );
    }
}


// ***********************
//
void        AnalogWatch::NodeRemovedHandler  ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeRemovedEvent::Type() )
        return;

    NodeRemovedEventPtr removedEvt = std::static_pointer_cast< NodeRemovedEvent >( evt );
    NodeRemoved( removedEvt->ParentNode.get(), removedEvt->RemovedNode.get() );
}

// ***********************
//
void        AnalogWatch::NodeMovedHandler   ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeMovedEvent::Type() )
        return;

    NodeMovedEventPtr movedEvent = std::static_pointer_cast< NodeMovedEvent >( evt );
    
    if( movedEvent->SrcParentNode == movedEvent->DstParentNode )
        return;

    NodeRemoved( movedEvent->SrcParentNode.get(), movedEvent->Node.get() );
}

// ***********************
//
void        AnalogWatch::NodeRemoved        ( model::BasicNode * parent, model::BasicNode * node )
{
    if( auto parentNode = m_parentNode.lock() )
    {
        // AnalogWatch uses only closest children.
        if( parent != parentNode.get() )
            return;

        if( node == m_hourNode )
        {
            m_hourNode = nullptr;
        }
        else if( node == m_minuteNode )
        {
            m_minuteNode = nullptr;
        }
        else if( node == m_secondsNode )
        {
            m_secondsNode = nullptr;
        }
    }
}

}   // nodelogic
}	// bv