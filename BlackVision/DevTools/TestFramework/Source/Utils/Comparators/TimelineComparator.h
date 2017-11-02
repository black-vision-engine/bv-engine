#pragma once

#include "Engine/Models/Timeline/Static/ConstTimeEvaluator.h"
#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"
#include "Engine/Models/Timeline/GlobalTimeline.h"
#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"

#include "PrimitiveComparator.h"

namespace bv { namespace model {

class TimelineComparator
{
public:

    static bool         CompareTimelines( const model::ITimeEvaluator & expected, const model::ITimeEvaluator & actual );
};

// ========================================================================= //
// Implementation
// ========================================================================= //

class TEST_ACCESSOR( ConstTimeEvaluator )
{
public:
    static bool         Compare( const ConstTimeEvaluator & expected, const ConstTimeEvaluator & actual )
    {
        if( expected.m_name != actual.m_name )
            return false;

        if( expected.m_timeVal != actual.m_timeVal )
            return false;

        return true;
    };
};

bool                    CompareConstTimelines( const ConstTimeEvaluator & expected, const ConstTimeEvaluator & actual )
{
    return TEST_ACCESSOR( ConstTimeEvaluator )::Compare( expected, actual );
}

class TEST_ACCESSOR( OffsetTimeEvaluator )
{
public:
    static bool         Compare( const OffsetTimeEvaluator & expected, const OffsetTimeEvaluator & actual )
    {
        if( expected.m_name != actual.m_name )
            return false;

        if( ! CompareFloats( expected.m_timeOffset, actual.m_timeOffset ) )
            return false;

        if( ! CompareFloats( expected.m_timeScale, actual.m_timeScale ) )
            return false;

        return true;
    }
};

bool                    CompareOffsetTimelines( const OffsetTimeEvaluator & expected, const OffsetTimeEvaluator & actual )
{
    return TEST_ACCESSOR( OffsetTimeEvaluator )::Compare( expected, actual );
}

bool                    CompareDefaultTimelines( const DefaultTimeline & expected, const DefaultTimeline & actual )
{
    if( expected.GetName() != actual.GetName() )
        return false;

    if( expected.GetDuration() != actual.GetDuration() )
        return false;

    if( expected.GetWrapBehaviorPre() != actual.GetWrapBehaviorPre() )
        return false;

    if( expected.GetWrapBehaviorPost() != actual.GetWrapBehaviorPost() )
        return false;

    return true;
}

bool         TimelineComparator::CompareTimelines( const ITimeEvaluator & expected, const ITimeEvaluator & actual )
{
    if( expected.GetType() != actual.GetType() )
        return false;

    if( expected.GetType() == ConstTimeEvaluator::Type() )
        return CompareConstTimelines( static_cast< const ConstTimeEvaluator &>( expected ), static_cast< const ConstTimeEvaluator &>( actual ) );
    else if( expected.GetType() == OffsetTimeEvaluator::Type() )
        return CompareOffsetTimelines( static_cast< const OffsetTimeEvaluator &>( expected ), static_cast< const OffsetTimeEvaluator &>( actual ) );
    else if( expected.GetType() == DefaultTimeline::Type() )
        return CompareDefaultTimelines( static_cast< const DefaultTimeline &>( expected ), static_cast< const DefaultTimeline &>( actual ) );

    assert( false );
    return false;
}

}   // model
}	// bv
