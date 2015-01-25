#pragma once

#include "Engine/Models/Timeline/TimeEvaluatorBase.h"


namespace bv { namespace model {

class ConstTimeEvaluator : public TimeEvaluatorBase< ITimeEvaluator >
{
private:

    typedef TimeEvaluatorBase< ITimeEvaluator > Parent;

private:

    TimeType        m_timeVal;

public:

            ConstTimeEvaluator                      ( const std::string & name, TimeType val );
            ~ConstTimeEvaluator                     ();

    void    SetConstTimeValue                       ( TimeType t );

    virtual void                SetGlobalTimeImpl   ( TimeType t ) override;
    virtual TimeType            GetLocalTime        () const override;

};

DEFINE_PTR_TYPE(ConstTimeEvaluator)

} //model
} //bv
