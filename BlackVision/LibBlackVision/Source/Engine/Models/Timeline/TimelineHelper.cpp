#include "stdafx.h"

#include "TimelineHelper.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Tools/StringHeplers.h"
#include "tools/PrefixHelper.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

const std::string   TimelineHelper::PATH_SEPARATOR     = "%";
const TimeType      TimelineHelper::INFINITE_DURATION  = std::numeric_limits< TimeType >::max();

// *********************************
//maybe should be moved into TimeEvalFactory or sth
ITimeEvaluatorPtr       TimelineHelper::CreateTimeEvaluator             ( const std::string & name, TimelineType type )
{
    switch ( type )
    {
    case TimelineType::TT_DEFAULT:
        return CreateDefaultTimeline( name, INFINITE_DURATION, TimelineWrapMethod::TWM_CLAMP, TimelineWrapMethod::TWM_CLAMP );
    case TimelineType::TT_OFFSET:
        return CreateOffsetTimeEvaluator( name, 0.f, 1.f );
    case TimelineType::TT_CONST:
        return CreateConstTimeEvaluator( name, 0.f );
    default:
        return nullptr;
    }
}

// *********************************
//
OffsetTimeEvaluatorPtr	TimelineHelper::CreateOffsetTimeEvaluator       ( const std::string & name, TimeType startTime, TimeType scale )
{
    return OffsetTimeEvaluator::Create( name, -startTime, scale ); 
}

// *********************************
//
ConstTimeEvaluatorPtr	TimelineHelper::CreateConstTimeEvaluator        ( const std::string & name, TimeType timeVal )
{
    return ConstTimeEvaluator::Create( name, timeVal ); 
}

// *********************************
//
DefaultTimelinePtr		TimelineHelper::CreateDefaultTimeline           ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod )
{
    assert( duration > TimeType( 0.0 ) );
    return DefaultTimeline::Create( name, duration, preMethod, postMethod );
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
bool					IsSceneRootTimeline( ITimeEvaluatorPtr timeline )
{
    return ( timeline->GetType() == OffsetTimeEvaluator::Type() ); // FIXME: not really the best implementation
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
    {
        if( IsSceneRootTimeline( timeline ) )
        {
            assert( false );
            continue;
        }
        index = std::max( index, GetIndex( destTimeline, timeline->GetName() ) );
    }

    for( auto timeline : timelines )
    {
        if( IsSceneRootTimeline( timeline ) )
            continue;

        auto clone = Clone( timeline );
        clone->SetName( PrefixHelper::PrefixCopy( index ) + timeline->GetName() );
        destTimeline->AddChild( clone );
    }

    return index;
}

// ***********************
//
std::string              TimelineHelper::GetSceneName                    ( const ITimeEvaluator * timeline )
{
    auto rootTimeline = TimelineManager::GetInstance()->GetRootTimeline();
    auto & scenesTimelines = rootTimeline->GetChildren();
    for( auto & sceneTimeline : scenesTimelines )
    {
        auto & childTimelines = sceneTimeline->GetChildren();
        for( auto & child : childTimelines )
        {
            if( child.get() == timeline )
                return sceneTimeline->GetName();
        }
    }
    return "";
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
ITimeEvaluatorPtr       TimelineHelper::GetTimeEvaluator           ( const std::string & timelinePath, ITimeEvaluatorPtr parentTimeline )
{
    auto path = Split( timelinePath, PATH_SEPARATOR );
    if( path.size() == 1 )
    {
        return FindTimelineByName( timelinePath, parentTimeline );
    }
    else
    {
        auto nextParent = FindTimelineByName( path[ 0 ], parentTimeline );
        if( nextParent )
        {
            path.erase( path.begin() );
            return GetTimeEvaluator( CombineTimelinePath( path ), nextParent );
        }
        else
        {
            return nullptr;
        }
    }
}

// *********************************
// FIXME: requires RTTI, reimplement it later on
ITimelinePtr            TimelineHelper::GetTimeline                     ( const std::string & timelinePath, ITimeEvaluatorPtr parentTimeline )
{
    auto path = Split( timelinePath, PATH_SEPARATOR );
    if( path.size() == 1 )
    {
        return std::dynamic_pointer_cast< ITimeline >( FindTimelineByName( timelinePath, parentTimeline ) );
    }
    else
    {
        auto nextParent = FindTimelineByName( path[ 0 ], parentTimeline );
        if( nextParent )
        {
            path.erase( path.begin() );
            return GetTimeline( CombineTimelinePath( path ), nextParent );
        }
        else
        {
            return nullptr;
        }
    }
}

// *********************************
//
std::string             TimelineHelper::GetTimelinePath                 ( ITimeEvaluatorPtr timeline, ITimeEvaluatorPtr parentTimeline )
{
    for( auto child : parentTimeline->GetChildren() )
    {
        if( child == timeline )
        {
            return timeline->GetName();
        }
        else
        {
            auto path = GetTimelinePath( timeline, child );
            if( path != "" )
            {
                return CombineTimelinePath( child->GetName(), path );
            }
        }
    }

    return "";
}

// *********************************
//
std::string              TimelineHelper::CombineTimelinePath            ( const std::vector< std::string > & strVec )
{
    return Join( strVec, PATH_SEPARATOR );
}

// *********************************
//
std::string              TimelineHelper::CombineTimelinePath            ( const std::string & pathA, const std::string & pathB )
{
    return pathA + PATH_SEPARATOR + pathB;
}

} //model
} //bv
