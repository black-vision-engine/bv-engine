#include "DefaultTimeline.h"


namespace bv { namespace model {

DefaultTimeline::DefaultTimeline     ( const std::string & name, TimeType duration, ITimeEvaluator * parent )
    : m_timeEvalImpl( duration )
{
}

} //model
} //bv
