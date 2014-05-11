#pragma once

#include "Engine/Models/Timeline/TimeEvaluatorBase.h"


namespace bv { namespace model {

class ConstTimeEvaluator : public TimeEvaluatorBase
{
private:

    TimeType        m_timeVal;

public:

            ConstTimeEvaluator                  ( const std::string & name, TimeType val );
            ~ConstTimeEvaluator                 ();

    void    SetConstTimeValue                   ( TimeType t );

    virtual void                SetGlobalTime   ( TimeType t ) override;
    virtual TimeType            GetLocalTime    () const override;

};

} //model
} //bv
