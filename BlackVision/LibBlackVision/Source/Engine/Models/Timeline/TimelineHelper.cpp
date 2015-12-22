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
UInt32                  GetIndex( ITimeEvaluatorPtr parent, std::string name )
{
    assert( false );
    return 0;
}

// *********************************
//
UInt32					TimelineHelper::CopyTimelines					( ITimeEvaluatorPtr destTimeline, const std::vector< ITimeEvaluatorPtr > & timelines )
{
    UInt32 index = 0;
	
    for( auto timeline : timelines )
        index = std::max( index, GetIndex( destTimeline, timeline->GetName() ) );

    for( auto timeline : timelines )
    {
        assert( false );
        //auto clone = timeline->Clone( index );
    }

	return index;
}

// *********************************
//
std::string				TimelineHelper::GetParentNodePath				( const std::string & timelinePath )
{
    auto path = Split( timelinePath, "/" );

	std::string parentPath = "";
	for( unsigned int i = 0; i < path.size() - 1; ++i )
	{
		parentPath += path[ i ];
		if( i < path.size() - 2 )
			parentPath += "/";
	}
	return parentPath;
}

// *********************************
//
ITimeEvaluatorPtr       FindTimelineByName             ( const std::string & name, ITimeEvaluatorPtr root )
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
ITimeEvaluatorPtr       TimelineHelper::GetTimeEvaluator           ( const std::string & name, ITimeEvaluatorPtr parentTimeline )
{
    auto path = Split( name, "/" );
    if( path.size() == 1 )
        return FindTimelineByName( name, parentTimeline );
    else
    {
        auto nextParent = FindTimelineByName( path[ 0 ], parentTimeline );
        if( nextParent )
        {
            path.erase( path.begin() );
            return GetTimeEvaluator( Join( path, "/"), nextParent );
        }
        else
            return nullptr;
    }
}

// *********************************
// FIXME: requires RTTI, reimplement it later on
ITimelinePtr            TimelineHelper::GetTimeline                     ( const std::string & name, ITimeEvaluatorPtr parentTimeline )
{
	auto path = Split( name, "/" );
    if( path.size() == 1 )
		return std::dynamic_pointer_cast< ITimeline >( FindTimelineByName( name, parentTimeline ) );
    else
    {
        auto nextParent = FindTimelineByName( path[ 0 ], parentTimeline );
        if( nextParent )
        {
            path.erase( path.begin() );
            return GetTimeline( Join( path, "/"), nextParent );
        }
        else
            return nullptr;
    }
}

// *********************************
//
std::string             TimelineHelper::GetTimelinePath                 ( ITimeEvaluatorPtr timeline, ITimeEvaluatorPtr parentTimeline )
{
    for( auto child : parentTimeline->GetChildren() )
        if( child == timeline )
            return timeline->GetName();
        else
        {
            auto path = GetTimelinePath( timeline, child );
            if( path != "" )
                return child->GetName() + "/" + path;
        }
    return "";
}


} //model
} //bv
