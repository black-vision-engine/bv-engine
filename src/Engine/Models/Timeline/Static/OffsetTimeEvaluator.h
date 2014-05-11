#pragma once

#include "Engine/Models/Timeline/TimeEvaluatorBase.h"


namespace bv { namespace model {

class OffsetTimeEvaluator : public TimeEvaluatorBase
{
private:

    TimeType        m_timeOffset;
    TimeType        m_globalTime;

public:

            OffsetTimeEvaluator                 ( const std::string & name, TimeType offsetTime );
            ~OffsetTimeEvaluator                ();

    void    SetTimeOffset                       ( TimeType t );

    virtual void                SetGlobalTime   ( TimeType t ) override;
    virtual TimeType            GetLocalTime    () const override;

};

} //model
} //bv
