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

    ITimeEvaluatorPtr       GetTimeEvaluator                ( const std::string & name );
    ITimeEvaluatorPtr       GetTimeEvaluator                ( const std::string & name, ITimeEvaluatorPtr parentTimeline );

    ITimelinePtr            GetTimeline                     ( const std::string & name );
    ITimelinePtr            GetTimeline                     ( const std::string & name, ITimeEvaluatorPtr parentTimeline );

    bool                    AddTimeline                     ( ITimeEvaluatorPtr timeline );
    bool                    AddTimelineToTimeline           ( ITimeEvaluatorPtr timeline, ITimeEvaluatorPtr parentTimeline );
    bool                    AddTimelineToTimeline           ( ITimeEvaluatorPtr timeline, const std::string & parentName );

    bool                    RemoveTimelineFromTimeline      ( ITimeEvaluatorPtr timeline, ITimeEvaluatorPtr parentTimeline );
    bool                    RemoveTimelineFromTimeline      ( ITimeEvaluatorPtr timeline, const std::string & parentName );
    bool                    RemoveTimelineFromTimeline      ( const std::string & name, ITimeEvaluatorPtr parentTimeline );
    bool                    RemoveTimelineFromTimeline      ( const std::string & name, const std::string & parentName );

    bool                    RemoveAllChildren               ( ITimeEvaluatorPtr timeline );
    bool                    RemoveAllChildren               ( const std::string & name );

    IParamSet *             GetRegisteredParameters         ( ITimeEvaluatorPtr timeline );
    IParamSet *             GetRegisteredParameters         ( const std::string & name );

    bool                    AddParamToTimeline              ( IParameterPtr param, const std::string & timelineName );
    bool                    AddParamToTimeline              ( IParameterPtr param, ITimeEvaluatorPtr timeline );

    unsigned int            RemoveParamFromTimeline         ( const std::string & paramName, const std::string & timelineName );
    bool                    RemoveParamFromTimeline         ( IParameterPtr param, const std::string & timelineName );
    bool                    RemoveParamFromTimeline         ( IParameterPtr param, const ITimeEvaluatorPtr timeline );

private:

    ITimeEvaluatorPtr       FindTimelineByName              ( const std::string & name, ITimeEvaluatorPtr root );
    SimpleIParamSet *       GetParamSet                     ( ITimeEvaluatorPtr timeline );
    bool                    DeregisterParam                 ( IParameterPtr param, ITimeEvaluatorPtr timeline );

};

} //model
} //bv