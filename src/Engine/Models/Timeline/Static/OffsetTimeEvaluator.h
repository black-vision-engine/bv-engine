#pragma once

#include <memory>

#include "Engine/Models/Timeline/TimeEvaluatorBase.h"


namespace bv { namespace model {

class OffsetTimeEvaluator : public TimeEvaluatorBase< ITimeEvaluator >
{
private:

    typedef TimeEvaluatorBase< ITimeEvaluator > Parent;

private:

    TimeType        m_timeOffset;
    TimeType        m_globalTime;

public:

            OffsetTimeEvaluator                     ( const std::string & name, TimeType offsetTime );
            ~OffsetTimeEvaluator                    ();

    void    SetTimeOffset                           ( TimeType t );

    virtual void                SetGlobalTimeImpl   ( TimeType t ) override;
    virtual TimeType            GetLocalTime        () const override;

};

typedef std::shared_ptr< OffsetTimeEvaluator > OffsetTimeEvaluatorPtr;

} //model
} //bv
