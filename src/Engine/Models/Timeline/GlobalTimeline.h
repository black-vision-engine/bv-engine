#pragma once

#include "Engine/Models/Timeline/ITimeEvaluator.h"


namespace bv { namespace model {

class GlobalTimeline : public ITimeEvaluator
{
private:

    TimeType m_startTime;

public:

                        GlobalTimeline    ( TimeType startTime );
    virtual             ~GlobalTimeline   ();

    virtual TimeType    Evaluate    ( TimeType t )  const;

};

} //model
} //bv
