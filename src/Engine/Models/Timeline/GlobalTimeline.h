#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class GlobalTimeline : public ITimeEvaluator
{
private:

    TimeType m_startTime;

public:

                        GlobalTimeline      ( TimeType startTime );
    virtual             ~GlobalTimeline     ();

    virtual TimeType    Evaluate            ( TimeType t )  const;

    void                SetStartTime        ( TimeType startTime );

};

} //model
} //bv
