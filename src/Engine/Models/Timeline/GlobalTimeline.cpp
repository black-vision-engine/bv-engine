#include "GlobalTimeline.h"

namespace bv { namespace model {

// *********************************
//
GlobalTimeline::GlobalTimeline    ( TimeType startTime )
    : m_startTime( startTime )
{
}

// *********************************
//
GlobalTimeline::~GlobalTimeline   ()
{
}

// *********************************
//
TimeType    GlobalTimeline::Evaluate    ( TimeType t )  const
{
    return t - m_startTime;
}

} //model
} //bv
