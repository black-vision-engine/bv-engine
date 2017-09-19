#include "stdafx.h"

#include "TimelineManager.h"

#include "TimelineHelper.h"

#include "Serialization/SerializationHelper.h"

#include "Tools/StringHeplers.h"
#include <cassert>




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

TimelineManager* TimelineManager::instance = nullptr; // FIXME: this may be moved to static initilizer

const std::string       TimelineManager::DEFAULT_TIMELINE_NAME = "default";


// ***********************
//
const std::string &     TimelineManager::GetDefaultTimelineName()
{
    return DEFAULT_TIMELINE_NAME;
}


// *********************************
//
TimelineManager* TimelineManager::GetInstance                     ()
{
    return instance;
}

// *********************************
//
void             TimelineManager::SetInstance                     ( TimelineManager* i )
{
    instance = i;
}
    
// *********************************
//
TimelineManager::TimelineManager         ()
    : m_rootTimeline( nullptr )
{
}

// *********************************
//
TimelineManager::~TimelineManager        ()
{
    for( auto it = m_registeredParams.begin(); it != m_registeredParams.end(); ++it )
    {
        delete it->second;
    }
}

// *********************************
//
void            TimelineManager::Serialize                       ( ISerializer& sob ) const
{
    sob.EnterArray( "timelines" );

    for( auto child : m_rootTimeline->GetChildren() )
        child->Serialize( sob );

    sob.ExitChild();
}

// *********************************
//
ISerializablePtr TimelineManager::Create                          ( const IDeserializer& deser )
{
    auto tm = std::make_shared< model::TimelineManager >();    

    auto timelines = SerializationHelper::DeserializeProperties< model::TimeEvaluatorBase< model::ITimeEvaluator > >( deser, "timeline" );
    for( auto timeline : timelines )
    {
        tm->AddTimeline( timeline );
    }

    tm->RegisterRootTimeline( timelines[ 0 ] );

    return tm;
}

// *********************************
//
bool                    TimelineManager::AddStopEventToTimeline          ( ITimelinePtr timeline, const std::string & eventName, TimeType stopTime )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( model::TimelineEventStop::Create( eventName, stopTime, timeline.get() ) );
}

// *********************************
//
bool                    TimelineManager::AddLoopReverseEventToTimeline   ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime, unsigned int totalLoopCount )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( model::TimelineEventLoop::Create( eventName, eventTime, LoopEventAction::LEA_REVERSE, totalLoopCount, TimeType( 0.0 ), timeline.get() ) );
}

// *********************************
//
bool                    TimelineManager::AddLoopJumpEventToTimeline      ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime, unsigned int totalLoopCount, TimeType jumpToTime )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( model::TimelineEventLoop::Create( eventName, eventTime, LoopEventAction::LEA_GOTO, totalLoopCount, jumpToTime, timeline.get() ) );
}

// *********************************
//
bool                    TimelineManager::AddLoopRestartEventToTimeline   ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime, unsigned int totalLoopCount )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( model::TimelineEventLoop::Create( eventName, eventTime, LoopEventAction::LEA_RESTART, totalLoopCount, TimeType( 0.0 ), timeline.get() ) );
}

// *********************************
//
bool                    TimelineManager::AddNullEventToTimeline     ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( model::TimelineEventNull::Create( eventName, eventTime, timeline.get() ) );
}

bool                    TimelineManager::AddTriggerEventToTimeline       ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime, const std::string & triggerEvents )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( model::TimelineEventTrigger::Create ( eventName, eventTime, triggerEvents, timeline.get() ) );
}

// *********************************
//
void                    TimelineManager::RegisterRootTimeline       ( ITimeEvaluatorPtr root )
{
    m_rootTimeline = root;
}

// *********************************
//
ITimeEvaluatorPtr       TimelineManager::GetRootTimeline            ()
{
    return m_rootTimeline;
}

// *********************************
//
ITimeEvaluatorPtr       TimelineManager::GetTimeEvaluator           ( const std::string & timelinePath )
{
    return TimelineHelper::GetTimeEvaluator( timelinePath, m_rootTimeline );
}

ITimelinePtr            TimelineManager::GetTimeline                     ( const std::string & timelinePath )
{
    return TimelineHelper::GetTimeline( timelinePath, m_rootTimeline );
}

// *********************************
//
std::string             TimelineManager::GetTimelinePath                 ( ITimeEvaluatorPtr timeline )
{
    return TimelineHelper::GetTimelinePath( timeline, m_rootTimeline );
}


// *********************************
//
bool                    TimelineManager::AddTimeline                     ( ITimeEvaluatorPtr timeline )
{
    assert( timeline != nullptr );

    if( m_rootTimeline != nullptr )
    {
        return AddTimelineToTimeline( timeline, m_rootTimeline );
    }
    else
    {
        RegisterRootTimeline( timeline );
        return true;
    }
}

// *********************************
//
bool                    TimelineManager::AddTimelineToTimeline           ( ITimeEvaluatorPtr timeline, ITimeEvaluatorPtr parentTimeline )
{
    assert( parentTimeline != nullptr );
    assert( timeline != nullptr );

    parentTimeline->AddChild( timeline );

    return true;    
}

// *********************************
//
bool                    TimelineManager::AddTimelineToTimeline           ( ITimeEvaluatorPtr timeline, const std::string & parentName )
{
    assert( timeline != nullptr );

    ITimeEvaluatorPtr parent = GetTimeEvaluator( parentName );
   
    if( parent != nullptr )
    {
        return AddTimelineToTimeline( timeline, parent );
    }

    return false;
}

// *********************************
//
bool                    TimelineManager::RemoveTimelineFromTimeline      ( ITimeEvaluatorPtr timeline, ITimeEvaluatorPtr parentTimeline )
{
    assert( timeline != nullptr );
    assert( parentTimeline != nullptr );
    
    return parentTimeline->RemoveChild( timeline );
}

// *********************************
//
bool                    TimelineManager::RemoveTimelineFromTimeline      ( ITimeEvaluatorPtr timeline, const std::string & parentName )
{
    ITimeEvaluatorPtr parent = GetTimeEvaluator( parentName );

    if( parent != nullptr )
    {
        return RemoveTimelineFromTimeline( timeline, parent );
    }

    return false;
}

// *********************************
//
bool                    TimelineManager::RemoveTimelineFromTimeline      ( const std::string & name, ITimeEvaluatorPtr parentTimeline )
{
    ITimeEvaluatorPtr timeline = GetTimeEvaluator( name );

    if( timeline != nullptr )
    {
        return RemoveTimelineFromTimeline( timeline, parentTimeline );
    }

    return false;
}

// *********************************
//
bool                    TimelineManager::RemoveTimelineFromTimeline      ( const std::string & name, const std::string & parentName )
{
    ITimeEvaluatorPtr parentTimeline = GetTimeEvaluator( parentName );
    ITimeEvaluatorPtr timeline = GetTimeEvaluator( name );

    if( parentTimeline != nullptr && timeline != nullptr )
    {
        return RemoveTimelineFromTimeline( timeline, parentTimeline );
    }

    return false;
}

// *********************************
//
bool                    TimelineManager::RemoveAllChildren               ( ITimeEvaluatorPtr timeline )
{
    if( timeline != nullptr )
    {
        std::vector< ITimeEvaluatorPtr > ccopy = timeline->GetChildren();

        for( auto child : ccopy )
        {
            timeline->RemoveChild( child );

            RemoveAllChildren( child );
        }

        return true;
    }

    return false;
}

// *********************************
//
bool                    TimelineManager::RemoveAllChildren              ( const std::string & name )
{
    auto timeline = GetTimeEvaluator( name );

    return RemoveAllChildren( timeline );
}

// *********************************
//
IParamSet *             TimelineManager::GetRegisteredParameters        ( ITimeEvaluatorPtr timeline )
{
    if( timeline != nullptr )
    {
        return GetParamSet( timeline );
    }

    return nullptr;
}

// *********************************
//
IParamSet *             TimelineManager::GetRegisteredParameters        ( const std::string & name )
{
    return GetRegisteredParameters( GetTimeEvaluator( name ) );
}

// *********************************
//
bool                    TimelineManager::AddParamToTimeline             ( IParameterPtr param, const std::string & timelineName )
{
    assert( param != nullptr );

    auto timeline = GetTimeEvaluator( timelineName );

    return AddParamToTimeline( param, timeline );
}

// *********************************
//
bool                    TimelineManager::AddParamToTimeline             ( IParameterPtr param, ITimeEvaluatorPtr timeline )
{
    assert( param != nullptr );

    if( timeline != nullptr )
    {
        RemoveParamFromTimeline( param, param->GetTimeEvaluator() );

        auto it = m_registeredParams.find( timeline.get() );

        if( it == m_registeredParams.end() )
        {
            auto paramSet = new SimpleIParamSet();

            m_registeredParams[ timeline.get() ] = paramSet;

            it = m_registeredParams.find( timeline.get() );

            assert( it != m_registeredParams.end() );
        }
    
        param->SetTimeEvaluator( timeline );

        it->second->AddParameter( param );

        return true;
    }

    return false;
}

// *********************************
//
SizeType            TimelineManager::RemoveParamFromTimeline        ( const std::string & paramName, const std::string & timelineName )
{
    auto timeline = GetTimeEvaluator( timelineName );
    auto params = GetParamSet( timeline );

    if( params != nullptr )
    {
        std::vector< IParameterPtr > paramBuf;

        for( auto param : params->GetParameters() )
        {
            if( param->GetName() == paramName )
            {
                paramBuf.push_back( param );
            }
        }

        SizeType numParams = paramBuf.size();

        for( auto param : paramBuf )
        {
            RemoveParamFromTimeline( param, timeline );
        }
  
        return numParams;
    }

    return 0;
}

// *********************************
//
bool                    TimelineManager::RemoveParamFromTimeline        ( IParameterPtr param, const std::string & timelineName )
{
    auto timeline = GetTimeEvaluator( timelineName );
    
    return RemoveParamFromTimeline( param, timeline );
}

// *********************************
//
bool                    TimelineManager::RemoveParamFromTimeline        ( IParameterPtr param, const ITimeEvaluatorPtr timeline )
{
    assert( param != nullptr );

    if( timeline != nullptr )
    {
        return DeregisterParam( param, timeline );
    }

    return false;
}

// *********************************
//
SimpleIParamSet *       TimelineManager::GetParamSet                     ( ITimeEvaluatorPtr timeline )
{
    auto it = m_registeredParams.find( timeline.get() );

    if( it != m_registeredParams.end() )
    {
        return it->second;
    }

    return nullptr;
}

// *********************************
//
bool                    TimelineManager::DeregisterParam                 ( IParameterPtr param, ITimeEvaluatorPtr timeline )
{
    if( param->GetTimeEvaluator() == timeline )
    {
        auto it = m_registeredParams.find( timeline.get() );

        assert( it != m_registeredParams.end() );

        it->second->RemoveParameter( param );

        if( it->second->GetParameters().size() == 0 )
        {
            delete it->second;

            m_registeredParams.erase( it );
        }

        param->SetTimeEvaluator( nullptr );

        return true;
    }

    return false;
}

} //model
} //bv
