#pragma once

#include <vector>
#include <hash_map>
#include <string>

namespace bv { namespace model {

class ITimeEvaluator;

class TimelineManager
{
private:

    static std::vector< const ITimeEvaluator * >   m_timelines;
    static std::hash_map< std::string, const ITimeEvaluator * > m_mappedTimelines;

public:

    static void RegisterTimeline( const std::string & name, const ITimeEvaluator * timeline );
    static void Cleanup         ();

};

} //model
} //bv
