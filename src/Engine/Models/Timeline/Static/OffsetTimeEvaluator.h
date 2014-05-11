#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class OffsetTimeEvaluator : public ITimeEvaluator
{
private:

    std::string     m_name;

    TimeType        m_timeOffset;
    TimeType        m_globalTime;

public:

            OffsetTimeEvaluator                 ( const std::string & name, TimeType offsetTime );
            ~OffsetTimeEvaluator                ();

    void    SetTimeOffset                       ( TimeType t );

    virtual const std::string & GetName         () const override;

    virtual void                SetGlobalTime   ( TimeType t ) override;
    virtual TimeType            GetLocalTime    () const override;

};

} //model
} //bv
