#include "TimelineManager.h"

#include <cassert>


namespace bv { namespace model {

// *********************************
//
TimelineManager::TimelineManager         ()
{
}

// *********************************
//
TimelineManager::~TimelineManager        ()
{
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
bool                    TimelineManager::AddStopEventToTimeline              ( ITimelinePtr timeline, const std::string & eventName, TimeType stopTime )
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
bool                    TimelineManager::AddNullEventToTimeline          ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime )
{
    assert( timeline != nullptr );

    return timeline->AddKeyFrame( new model::TimelineEventNull( eventName, eventTime, timeline.get() ) );
}

//// *********************************
////
//void                    TimelineManager::RegisterRootTimeline    ( ITimeEvaluatorPtr root )
//{
//    //FIXME: implement
//}
//
//// *********************************
////
//ITimeEvaluatorPtr       TimelineManager::GetRootTimeline         ()
//{
//    //FIXME: implement
//    return nullptr;
//}
//
//// *********************************
////
//ITimeEvaluatorPtr       TimelineManager::GetTimeline             ( const std::string & name )
//{
//    //FIXME: implement
//    return nullptr;
//}
//
//// *********************************
////
//IParamSet *             TimelineManager::GetRegisteredParameters ( ITimeEvaluatorPtr timeline )
//{
//    //FIXME: implement
//    return nullptr;
//
//    auto it = m_registeredParams.find( timeline.get() );
//
//    if( it != m_registeredParams.end() )
//    {
//        return it->second;
//    }
//    
//    return nullptr;
//}
//
//// *********************************
////
//IParamSet *             TimelineManager::GetRegisteredParameters ( const std::string & name )
//{
//    return GetRegisteredParameters( GetTimeline( name ) );
//}
//
//
//// *********************************
////
//bool                    TimelineManager::RegisterOffsetTimeline  ( const std::string & name, TimeType offsetTime, std::string & parentName )
//{
//    //FIXME: implement
//    return false;
//}
//
//// *********************************
////
//bool                    TimelineManager::RegisterConstTimeline   ( const std::string & name, TimeType constTime, std::string & parentName )
//{
//    //FIXME: implement
//    return false;
//}
//
//// *********************************
////
//bool                    TimelineManager::RegisterTimeline        ( ITimeEvaluatorPtr timeline )
//{
//    //FIXME: implement
//    return false;
//}
//
//// *********************************
////
//bool                    TimelineManager::AddParamToTimeline      ( IParameter * param, const std::string & timelineName )
//{
//    //FIXME: implement
//    return false;
//
//    auto timeline = GetTimeline( timelineName );
//
//    if( timeline != nullptr )
//    {
//        return AddParamToTimelineImpl( param, timeline );
//    }
//
//    return false;
//}
//
//// *********************************
////
//bool                    TimelineManager::AddParamToTimeline      ( IParameter * param, ITimeEvaluatorPtr timeline )
//{
//    //FIXME: implement
//    return false;
//
//    auto tl = GetTimeline( timeline-> GetName() );
//
//
//    if( tl != nullptr )
//    {
//        return AddParamToTimelineImpl( param, tl );
//    }
//
//    if ( !RegisterTimeline( timeline ) )
//    {
//        assert( false );
//
//        return false;
//    }
//
//    return AddParamToTimeline( param, timeline );
//}
//
//// *********************************
////
//unsigned int            TimelineManager::RemoveFromTimeline      ( const std::string & paramName, const std::string & timelineName )
//{
//    //FIXME: implement
//    return 0;
//
//    auto paramSet = GetSimpleIParamSet( timelineName );
//
//    if( paramSet )
//    {
//        return paramSet->RemoveParameters( paramName );
//    }
//
//    return 0;
//}
//
//// *********************************
////
//bool                    TimelineManager::RemoveFromTimeline      ( IParameter * param, const std::string & timelineName )
//{
//    //FIXME: implement
//    return false;
//
//    auto paramSet = GetSimpleIParamSet( timelineName );
//
//    if( paramSet )
//    {
//        paramSet->RemoveParameter( param );
//    }
//
//    return false;
//}
//
//// *********************************
////
//SimpleIParamSet *       TimelineManager::GetSimpleIParamSet      ( const std::string & timelineName )
//{
//    //FIXME: implement
//    return nullptr;
//
//    auto tl = GetTimeline( timelineName );
//
//    if( tl != nullptr )
//    {
//        //assert( m_registeredParams.find( tl ) != m_registeredParams.end() );
//
//        //return m_registeredParams[ tl ];
//        return nullptr;
//    }
//
//    return nullptr;
//}
//
//// *********************************
////
//bool                    TimelineManager::AddParamToTimelineImpl ( IParameter * param, ITimeEvaluatorPtr timeline )
//{
//    //FIXME: implement
//    return false;
//
//    auto paramSet = m_registeredParams[ timeline.get() ];
//
//    assert( paramSet != nullptr );
//
//    bool bAdded = paramSet->AddParameter( param );
//    
//    if ( bAdded )
//    {
//        //SetParamTimeline( param, timeline );
//    }
//
//    return bAdded;
//}

} //model
} //bv
