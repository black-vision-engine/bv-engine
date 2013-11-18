#pragma once

#include <vector>


namespace bv { namespace model {

class ITimeEvaluator;

class TimelineManager
{
private:

    static std::vector< const ITimeEvaluator * >   m_timelines;

public:

    static void RegisterTimeline( const ITimeEvaluator * timeline );
    static void Cleanup         ();

};

} //model
} //bv
