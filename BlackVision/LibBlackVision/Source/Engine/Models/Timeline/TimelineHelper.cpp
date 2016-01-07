#include "TimelineHelper.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Tools/StringHeplers.h"
#include "tools/PrefixHelper.h"
#include "Serialization/CloneViaSerialization.h"
#include <cassert>
#include <algorithm>

namespace bv { namespace model {

// *********************************
//maybe should be moved into TimeEvalFactory or sth
ITimeEvaluatorPtr       TimelineHelper::CreateTimeEvaluator             ( const std::string & name, TimelineType type )
{
    switch ( type )
    {
    case TimelineType::TT_DEFAULT:
        return CreateDefaultTimeline( name, 1.f, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    case TimelineType::TT_OFFSET:
        return CreateOffsetTimeEvaluator( name, 0.f );
    case TimelineType::TT_CONST:
        return CreateConstTimeEvaluator( name, 0.f );
    default:
        return nullptr;
    }
}

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
Int32                  GetIndex( ITimeEvaluatorPtr parent, std::string name )
{
    Int32 num = -1;
    for( auto & timeline : parent->GetChildren() )
    {
        auto res = PrefixHelper::MatchNames( name, timeline->GetName() );
        if( num < res )
        {
            num = res;
        }
    }

    return num + 1;
}

// *********************************
//
UInt32					TimelineHelper::CopyTimelines					( ITimeEvaluatorPtr destTimeline, const std::vector< ITimeEvaluatorPtr > & timelines )
{
    if( timelines.size() == 0 )
    {
        assert( false );
        return UInt32( -1 );
    }

    Int32 index = 0;
	
    for( auto timeline : timelines )
        index = std::max( index, GetIndex( destTimeline, timeline->GetName() ) );

    for( auto timeline : timelines )
    {
        auto clone = Clone( timeline );
        clone->SetName( PrefixHelper::PrefixCopy( index ) + timeline->GetName() );
        destTimeline->AddChild( clone );
    }

	return index;
}

// ***********************
//
std::string              TimelineHelper::GetSceneName                    ( const ITimeEvaluator* timeline )
{
    auto rootTimeline = TimelineManager::GetInstance()->GetRootTimeline();
    auto& scenesTimelines = rootTimeline->GetChildren();
    for( auto& sceneTimeline : scenesTimelines )
    {
        auto& childTimelines = sceneTimeline->GetChildren();
        for( auto& child : childTimelines )
        {
            if( child.get() == timeline )
                return sceneTimeline->GetName();
        }
    }
    return "";
}

// *********************************
//
std::string				TimelineHelper::GetSceneName        			( const std::string & timelinePath )
{
    auto path = Trim( timelinePath, "/" );
    auto names = Split( path, "/" );
    if( !names.empty() )
    {
        return names[ 0 ];
    }
    return "";
}

// *********************************
//
std::string				TimelineHelper::GetParentTimelinePath           ( const std::string & timelinePath )
{
    auto path = Trim( timelinePath, "%" );
    auto names = Split( timelinePath, "%" );

	std::string parentPath = "";
	for( unsigned int i = 0; i < names.size() - 1; ++i )
	{
		parentPath += names[ i ];
		if( i < names.size() - 2 )
			parentPath += "%";
	}
	return parentPath;
}

// *********************************
//
ITimeEvaluatorPtr       TimelineHelper::FindTimelineByName          ( const std::string & name, ITimeEvaluatorPtr root )
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
    auto path = Split( name, "%" );
    if( path.size() == 1 )
        return FindTimelineByName( name, parentTimeline );
    else
    {
        auto nextParent = FindTimelineByName( path[ 0 ], parentTimeline );
        if( nextParent )
        {
            path.erase( path.begin() );
            return GetTimeEvaluator( Join( path, "%"), nextParent );
        }
        else
            return nullptr;
    }
}

// *********************************
// FIXME: requires RTTI, reimplement it later on
ITimelinePtr            TimelineHelper::GetTimeline                     ( const std::string & name, ITimeEvaluatorPtr parentTimeline )
{
	auto path = Split( name, "%" );
    if( path.size() == 1 )
		return std::dynamic_pointer_cast< ITimeline >( FindTimelineByName( name, parentTimeline ) );
    else
    {
        auto nextParent = FindTimelineByName( path[ 0 ], parentTimeline );
        if( nextParent )
        {
            path.erase( path.begin() );
            return GetTimeline( Join( path, "%"), nextParent );
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
                return child->GetName() + "%" + path;
        }
    return "";
}


} //model
} //bv
