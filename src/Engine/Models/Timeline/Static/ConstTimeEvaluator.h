#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class ConstTimeEvaluator : public ITimeEvaluator
{
private:

    std::string     m_name;

    TimeType        m_timeVal;

public:

            ConstTimeEvaluator                  ( const std::string & name, TimeType val );
            ~ConstTimeEvaluator                 ();

    void    SetConstTimeValue                   ( TimeType t );

    virtual const std::string & GetName         () const override;

    virtual void                SetGlobalTime   ( TimeType t ) override;
    virtual TimeType            GetLocalTime    () const override;

};

} //model
} //bv
