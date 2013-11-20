#pragma once

#include <vector>
#include <string>

namespace bv { namespace model {

class ITimeEvaluator;

class TimelineManager
{
private:

    static std::vector< const ITimeEvaluator * >   m_timelines;
    //static std::vector< std::string >              m_names;

public:

    static void RegisterTimeline( const std::string & name, const ITimeEvaluator * timeline );
    static void Cleanup         ();

};

} //model
} //bv
