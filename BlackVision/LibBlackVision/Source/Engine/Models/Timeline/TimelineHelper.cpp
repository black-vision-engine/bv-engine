#include "TimelineHelper.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Tools/StringHeplers.h"
#include <cassert>


namespace bv { namespace model {

// *********************************
//
ITimeEvaluatorPtr       TimelineHelper::CreateOffsetTimeEvaluator      ( const std::string & name, TimeType startTime )
{
    return CreateOffsetTimeEvaluatorImpl( name, startTime );
}

// *********************************
//
ITimeEvaluatorPtr       TimelineHelper::CreateConstTimeEvaluator       ( const std::string & name, TimeType timeVal )
{
    return CreateConstTimeEvaluatorImpl( name, timeVal );
}

// *********************************
//
ITimelinePtr            TimelineHelper::CreateDefaultTimeline          ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    return CreateDefaultTimelineImpl( name, duration, preMethod, postMethod );
}

// *********************************
//
OffsetTimeEvaluatorPtr	TimelineHelper::CreateOffsetTimeEvaluatorImpl  ( const std::string & name, TimeType startTime )
{
    return std::make_shared< OffsetTimeEvaluator >( name, -startTime ); 
}

// *********************************
//
ConstTimeEvaluatorPtr	TimelineHelper::CreateConstTimeEvaluatorImpl   ( const std::string & name, TimeType timeVal )
{
    return std::make_shared< ConstTimeEvaluator >( name, timeVal ); 
}

// *********************************
//
DefaultTimelinePtr		TimelineHelper::CreateDefaultTimelineImpl      ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    assert( duration > TimeType( 0.0 ) );
    return std::make_shared< DefaultTimeline >( name, duration, preMethod, postMethod );
}

// *********************************
//
UInt32					TimelineHelper::CopyTimelines					( ITimeEvaluatorPtr destTimeline, const std::vector< ITimeEvaluatorPtr > & timelines )
{
	{ timelines; destTimeline; }

	//FIXME: copy timelines from vector and add them to destTimeline (prefix their names if necessary)
	assert( false );

	//return prefix number
	return 0;
}

// *********************************
//
std::string				TimelineHelper::GetParentNodePath				( const std::string & timelinePath )
{
    auto path = Split( timelinePath, "/" );

	std::string parentPath = "";
	for( int i = 0; i < path.size() - 1; ++i )
	{
		parentPath += path[ i ];
		if( i < path.size() - 2 )
			parentPath += "/";
	}
	return parentPath;
}

} //model
} //bv
