#include "TimelineManager.h"

#include "Engine/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

std::vector< const ITimeEvaluator * >   TimelineManager::m_timelines;

// *********************************
//
void TimelineManager::RegisterTimeline( const std::string & name, const ITimeEvaluator * timeline )
{
    //FIXME: do not let two identical timelines to be registered
    m_timelines.push_back( timeline );
    //m_names.push_back( name );
}

// *********************************
//
void TimelineManager::Cleanup         ()
{
    for( auto ti : m_timelines )
    {
        delete ti;
    }
}

} //model
} //bv
