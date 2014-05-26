#include "TimelineManager.h"

#include <cassert>


namespace bv { namespace model {

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
ITimeEvaluatorPtr       TimelineManager::CreateOffsetTimeEvaluator      ( const std::string & name, TimeType startTime )
{
    return CreateOffsetTimeEvaluatorImpl( name, startTime );
}

// *********************************
//
ITimeEvaluatorPtr       TimelineManager::CreateConstTimeEvaluator       ( const std::string & name, TimeType timeVal )
{
    return CreateConstTimeEvaluatorImpl( name, timeVal );
}

// *********************************
//
ITimelinePtr            TimelineManager::CreateDefaultTimeline          ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    return CreateDefaultTimelineImpl( name, duration, preMethod, postMethod );
}

// *********************************
//
OffsetTimeEvaluatorPtr  TimelineManager::CreateOffsetTimeEvaluatorImpl  ( const std::string & name, TimeType startTime )
{
    return OffsetTimeEvaluatorPtr( new OffsetTimeEvaluator( name, -startTime ) ); 
}

// *********************************
//
ConstTimeEvaluatorPtr   TimelineManager::CreateConstTimeEvaluatorImpl   ( const std::string & name, TimeType timeVal )
{
    return ConstTimeEvaluatorPtr( new ConstTimeEvaluator( name, timeVal ) ); 
}

// *********************************
//
DefaultTimelinePtr      TimelineManager::CreateDefaultTimelineImpl      ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    assert( duration > TimeType( 0.0 ) );
    auto timeline = DefaultTimelinePtr( new DefaultTimeline( name, duration, preMethod, postMethod ) );

    return timeline;
}

// *********************************
//
bool                    TimelineManager::AddStopEventToTimeline          ( ITimelinePtr timeline, const std::string & eventName, TimeType stopTime )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( new model::TimelineEventStop( eventName, stopTime, timeline.get() ) );
}

// *********************************
//
bool                    TimelineManager::AddLoopReverseEventToTimeline   ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime, unsigned int totalLoopCount )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( new model::TimelineEventLoop( eventName, eventTime, LoopEventAction::LEA_REVERSE, totalLoopCount, TimeType( 0.0 ), timeline.get() ) );
}

// *********************************
//
bool                    TimelineManager::AddLoopJumpEventToTimeline      ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime, unsigned int totalLoopCount, TimeType jumpToTime )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( new model::TimelineEventLoop( eventName, eventTime, LoopEventAction::LEA_GOTO, totalLoopCount, jumpToTime, timeline.get() ) );
}

// *********************************
//
bool                    TimelineManager::AddLoopRestartEventToTimeline   ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime, unsigned int totalLoopCount )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( new model::TimelineEventLoop( eventName, eventTime, LoopEventAction::LEA_RESTART, totalLoopCount, TimeType( 0.0 ), timeline.get() ) );
}

// *********************************
//
bool                    TimelineManager::AddNullEventToTimeline     ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( new model::TimelineEventNull( eventName, eventTime, timeline.get() ) );
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
ITimeEvaluatorPtr       TimelineManager::GetTimeline                ( const std::string & name )
{
    return FindTimelineByName( name, m_rootTimeline );
}

// *********************************
//
ITimeEvaluatorPtr       TimelineManager::GetTimeline                ( const std::string & name, ITimeEvaluatorPtr parentTimeline )
{
    if( parentTimeline != nullptr )
    {
        for( auto child : parentTimeline->GetChildren() )
        {
            auto retVal = FindTimelineByName( name, child );
            
            if( retVal != nullptr )
            {
                return retVal;
            }
        }
    }

    return nullptr;
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

    return false;
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

    ITimeEvaluatorPtr parent = GetTimeline( parentName );
   
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
    ITimeEvaluatorPtr parent = GetTimeline( parentName );

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
    ITimeEvaluatorPtr timeline = GetTimeline( name );

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
    ITimeEvaluatorPtr parentTimeline = GetTimeline( parentName );
    ITimeEvaluatorPtr timeline = GetTimeline( name );

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
    auto timeline = GetTimeline( name );

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
    return GetRegisteredParameters( GetTimeline( name ) );
}

// *********************************
//
bool                    TimelineManager::AddParamToTimeline             ( IParameter * param, const std::string & timelineName )
{
    assert( param != nullptr );

    auto timeline = GetTimeline( timelineName );

    return AddParamToTimeline( param, timeline );
}

// *********************************
//
bool                    TimelineManager::AddParamToTimeline             ( IParameter * param, ITimeEvaluatorPtr timeline )
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
unsigned int            TimelineManager::RemoveParamFromTimeline        ( const std::string & paramName, const std::string & timelineName )
{
    auto timeline = GetTimeline( timelineName );
    auto params = GetParamSet( timeline );

    if( params != nullptr )
    {
        std::vector< IParameter * > paramBuf;

        for( auto param : params->GetParameters() )
        {
            if( param->GetName() == paramName )
            {
                paramBuf.push_back( param );
            }
        }

        unsigned int numParams = paramBuf.size();

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
bool                    TimelineManager::RemoveParamFromTimeline        ( IParameter * param, const std::string & timelineName )
{
    auto timeline = GetTimeline( timelineName );
    
    return RemoveParamFromTimeline( param, timeline );
}

// *********************************
//
bool                    TimelineManager::RemoveParamFromTimeline        ( IParameter * param, const ITimeEvaluatorPtr timeline )
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
ITimeEvaluatorPtr       TimelineManager::FindTimelineByName             ( const std::string & name, ITimeEvaluatorPtr root )
{
    if( root != nullptr )
    {
        if( root->GetName() == name )
        {
            return root;
        }
        else
        {
            for( auto child : root->GetChildren() )
            {
                auto retTimeline = FindTimelineByName( name, child );

                if( retTimeline != nullptr )
                {
                    return retTimeline;
                }
            }
        }
    }

    return nullptr;
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
bool                    TimelineManager::DeregisterParam                 ( IParameter * param, ITimeEvaluatorPtr timeline )
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
