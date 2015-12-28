#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"
#include "Engine/Models/Timeline/Static/ConstTimeEvaluator.h"
#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"

#include "CoreDEF.h"


namespace bv { namespace model {

class TimelineHelper
{
public:

    static ITimeEvaluatorPtr		CreateOffsetTimeEvaluator       ( const std::string & name, TimeType startTime );
    static ITimeEvaluatorPtr		CreateConstTimeEvaluator        ( const std::string & name, TimeType timeVal );
    static ITimelinePtr				CreateDefaultTimeline           ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );

    static OffsetTimeEvaluatorPtr	CreateOffsetTimeEvaluatorImpl   ( const std::string & name, TimeType startTime );
    static ConstTimeEvaluatorPtr	CreateConstTimeEvaluatorImpl    ( const std::string & name, TimeType timeVal );
    static DefaultTimelinePtr		CreateDefaultTimelineImpl       ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );
    
	/** Make copy of timelines from vector and add them to destTimeline.
	@return Returns group prefix number. */
	static UInt32					CopyTimelines					( ITimeEvaluatorPtr destTimeline, const std::vector< ITimeEvaluatorPtr > & timelines );

	static std::string				GetParentNodePath				( const std::string & timelinePath );
	
    static ITimeEvaluatorPtr        GetTimeEvaluator                ( const std::string & name, ITimeEvaluatorPtr parentTimeline );
    static ITimelinePtr             GetTimeline                     ( const std::string & name, ITimeEvaluatorPtr parentTimeline );
    static std::string              GetTimelinePath                 ( ITimeEvaluatorPtr timeline, ITimeEvaluatorPtr parentTimeline );

};

} //model
} //bv
