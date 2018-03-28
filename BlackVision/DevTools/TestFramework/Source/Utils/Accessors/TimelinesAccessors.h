#pragma once

#include "Engine/Models/Timeline/Static/ConstTimeEvaluator.h"
#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"
#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"

#include "Utils/Comparators/PrimitiveComparator.h"

namespace bv {

namespace model {

class TEST_ACCESSOR( ConstTimeEvaluator )
{
public:
    static bool         Compare( const ConstTimeEvaluator & expected, const ConstTimeEvaluator & actual )
    {
        if( expected.m_name != actual.m_name )
            return false;

        if( ! CompareFloats( expected.m_timeVal, actual.m_timeVal ) )
            return false;

        return true;
    };
};

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


} // model

} //bv
