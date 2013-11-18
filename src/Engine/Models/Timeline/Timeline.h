#pragma once

#include "Engine/Models/Timeline/TimelineBase.h"


namespace bv { namespace model {

class Timeline : public TimelineBase
{
private:

    const ITimeEvaluator *    m_parent;

public:

                        Timeline    ( ITimeEvaluator * parent, TimeType startTime, TimeType endTime );
    virtual             ~Timeline   ();

    virtual TimeType    Evaluate    ( TimeType t )  const;

};

} //model
} //bv
