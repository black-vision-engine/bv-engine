#pragma once

#include <hash_map>
#include <vector>
#include <string>

#include "Engine/Models/Interfaces/ITimeEvaluator.h"

#include "System/BasicTypes.h"

#include "Engine/Models/Timeline/SimpleIParamSet.h"

#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"
#include "Engine/Models/Timeline/Static/ConstTimeEvaluator.h"

#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"

#include "Engine/Models/Timeline/Dynamic/TimelineEventLoop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventNull.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventStop.h"


namespace bv { namespace model {

class TimelineManager
{
private:

    std::hash_map< std::string, ITimeEvaluatorPtr >         m_timelinesMap;
    std::hash_map< ITimeEvaluator *, SimpleIParamSet * >    m_registeredParams;

    ITimeEvaluatorPtr                                       m_rootTimeline;

public:

                            TimelineManager                 ();
                            ~TimelineManager                ();

    ITimeEvaluatorPtr       CreateOffsetTimeEvaluator       ( const std::string & name, TimeType startTime );
    ITimeEvaluatorPtr       CreateConstTimeEvaluator        ( const std::string & name, TimeType timeVal );
    ITimelinePtr            CreateDefaultTimeline           ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );

    OffsetTimeEvaluatorPtr  CreateOffsetTimeEvaluatorImpl   ( const std::string & name, TimeType startTime );
    ConstTimeEvaluatorPtr   CreateConstTimeEvaluatorImpl    ( const std::string & name, TimeType timeVal );
    DefaultTimelinePtr      CreateDefaultTimelineImpl       ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );
    
    bool                    AddStopEventToTimeline          ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime );
    bool                    AddLoopReverseEventToTimeline   ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime, unsigned int totalLoopCount );
    bool                    AddLoopJumpEventToTimeline      ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime, unsigned int totalLoopCount, TimeType jumpToTime );
    bool                    AddLoopRestartEventToTimeline   ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime, unsigned int totalLoopCount );
    bool                    AddNullEventToTimeline          ( ITimelinePtr timeline, const std::string & eventName, TimeType eventTime );

    void                    RegisterRootTimeline            ( ITimeEvaluatorPtr root );

    ITimeEvaluatorPtr       GetRootTimeline                 ();

    ITimeEvaluatorPtr       GetTimeline                     ( const std::string & name );
    ITimeEvaluatorPtr       GetTimeline                     ( const std::string & name, ITimeEvaluatorPtr parentTimeline );

    bool                    AddTimeline                     ( ITimeEvaluatorPtr timeline );
    bool                    AddTimelineToTimeline           ( ITimeEvaluatorPtr timeline, ITimeEvaluatorPtr parentTimeline );
    bool                    AddTimelineToTimeline           ( ITimeEvaluatorPtr timeline, const std::string & parentName );

    bool                    RemoveTimelineFromTimeline      ( ITimeEvaluatorPtr timeline, ITimeEvaluatorPtr parentTimeline );
    bool                    RemoveTimelineFromTimeline      ( ITimeEvaluatorPtr timeline, const std::string & parentName );
    bool                    RemoveTimelineFromTimeline      ( const std::string & name, ITimeEvaluatorPtr parentTimeline );
    bool                    RemoveTimelineFromTimeline      ( const std::string & name, const std::string & parentName );

    bool                    RemoveAllChildren               ( ITimeEvaluatorPtr timeline );
    bool                    RemoveAllChildren               ( const std::string & name );

    IParamSet *             GetRegisteredParameters ( ITimeEvaluatorPtr timeline );
    IParamSet *             GetRegisteredParameters ( const std::string & name );

//    bool                    RegisterOffsetTimeline  ( const std::string & name, TimeType offsetTime, std::string & parentName );
//    bool                    RegisterConstTimeline   ( const std::string & name, TimeType constTime, std::string & parentName );
//
//    bool                    RegisterTimeline        ( ITimeEvaluatorPtr timeline );
//
//    bool                    AddParamToTimeline      ( IParameter * param, const std::string & timelineName );
//    bool                    AddParamToTimeline      ( IParameter * param, ITimeEvaluatorPtr timeline );
//
//    unsigned int            RemoveFromTimeline      ( const std::string & paramName, const std::string & timelineName );
//    bool                    RemoveFromTimeline      ( IParameter * param, const std::string & timelineName );
//
private:

    ITimeEvaluatorPtr       FindTimelineByName              ( const std::string & name, ITimeEvaluatorPtr root );
//    SimpleIParamSet *       GetSimpleIParamSet      ( const std::string & timelineName );
//    bool                    AddParamToTimelineImpl  ( IParameter * param, ITimeEvaluatorPtr timeline );
//
//    bool                    RegisterOffsetTimeline  ( const std::string & name, TimeType offsetTime, ITimeEvaluatorPtr parent );
//    bool                    RegisterConstTimeline   ( const std::string & name, TimeType constTime, ITimeEvaluatorPtr parent );

};

} //model
} //bv

//// *********************************
////
//bool                                DefaultTimeline::AddParameter       ( IParameter * param )
//{
//    if( std::find( m_registeredParameters.begin(), m_registeredParameters.end(), param ) == m_registeredParameters.end() )
//    {
//        m_registeredParameters.push_back( param );
//        
//        return true;
//    }
//
//    return false;
//}
//
//// *********************************
////
//bool                                DefaultTimeline::RemoveParameter    ( const IParameter * param )
//{
//    auto it = std::find( m_registeredParameters.begin(), m_registeredParameters.end(), param );
//
//    if( it != m_registeredParameters.end() )
//    {
//        m_registeredParameters.erase( it );
//
//        return true;
//    }
//
//    return false;    
//}
//
//// *********************************
////
//unsigned int                        DefaultTimeline::RemoveParameters   ( const std::string & name )
//{
//    unsigned int erasedElements = 0;
//
//    for( auto it = m_registeredParameters.begin(); it != m_registeredParameters.end(); )
//    {
//        if( name == (*it)->GetName() )
//        {
//            it = m_registeredParameters.erase( it );
//
//            ++erasedElements;
//        }
//        else
//        {
//            ++it;
//        }
//    }
//
//    return erasedElements;
//}
