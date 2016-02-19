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

    virtual void                Serialize           ( ISerializer& sob ) const;
    static ConstTimeEvaluator * Create              ( const IDeserializer& dob );

    void    SetConstTimeValue                       ( TimeType t );

    virtual void                SetGlobalTimeImpl   ( TimeType t ) override;
    virtual TimeType            GetLocalTime        () const override;

    virtual const std::string&  GetType             () override;
    static const std::string&   Type                ();
};

DEFINE_PTR_TYPE(ConstTimeEvaluator)

} //model
} //bv
