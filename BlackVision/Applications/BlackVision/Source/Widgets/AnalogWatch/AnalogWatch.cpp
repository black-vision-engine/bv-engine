#include "AnalogWatch.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Widgets/NodeLogicHelper.h"

// warning C4996: 'localtime': This function or variable may be unsafe
#pragma warning(disable : 4996)
#include <chrono>
#include <ctime>


namespace bv { namespace nodelogic
{

const std::string       AnalogWatch::m_type = "AnalogWatch";

const std::string       AnalogWatch::ACTION::START_WATCH        = "StartWatch";
const std::string       AnalogWatch::ACTION::CLEAR_WATCH        = "ClearWatch";

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
AnalogWatch::AnalogWatch             ( bv::model::BasicNodePtr parent, bv::model::ITimeEvaluatorPtr /*timeEvaluator*/ )
    :   m_parentNode( parent )
{}

// ***********************
//
AnalogWatch::~AnalogWatch()
{}

// ***********************
//
void                        AnalogWatch::Update			( TimeType /*t*/ )
{
    if( m_started )
    {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t( now );
    
        tm* localTime = localtime( &time );
    
        UpdateTime( m_hourNode, (float)localTime->tm_hour / 12.0f );
        UpdateTime( m_minuteNode, (float)localTime->tm_min / 60.0f );
        UpdateTime( m_secondsNode, (float)localTime->tm_sec / 60.0f );
    }
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        AnalogWatch::Serialize       ( ISerializer & ser ) const
{
    ser.EnterChild( "logic" );
        ser.SetAttribute( "type", m_type );
    ser.ExitChild();    // logic
}

// ***********************
//
AnalogWatchPtr              AnalogWatch::Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parentNode )
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
    int numChildren = m_parentNode->GetNumChildren();

    m_hourNode = numChildren > 1 ? m_parentNode->GetChild( 0 ) : nullptr;
    m_minuteNode = numChildren > 2 ? m_parentNode->GetChild( 1 ) : nullptr;
    m_secondsNode = numChildren > 3 ? m_parentNode->GetChild( 2 ) : nullptr;

    SetInitialPosition( m_hourNode );
    SetInitialPosition( m_minuteNode );
    SetInitialPosition( m_secondsNode );

    m_started = true;

    return true;
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
void        AnalogWatch::SetInitialPosition      ( bv::model::BasicNodePtr& node )
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
void        AnalogWatch::UpdateTime              ( bv::model::BasicNodePtr& node, float ratio )
{
    if( node )
    {
        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();
        transformParam->SetRotation( glm::vec3( 0.0f, 0.0f, -360.0f * ratio ), TimeType( 0.0f ) );
    }
}

// ***********************
//
void        AnalogWatch::ClearPosition           ( bv::model::BasicNodePtr& node )
{
    if( node )
    {
        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();
        transformParam->SetRotation( glm::vec3( 0.0f, 0.0f, 0.0f ), TimeType( 0.0f ) );
    }
}

}   // nodelogic
}	// bv