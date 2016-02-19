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
    TimeType        m_timeScale;
    TimeType        m_globalTime;

public:

                                                    OffsetTimeEvaluator                     ( const std::string & name, TimeType offsetTime, TimeType scale = 1 );
                                                    ~OffsetTimeEvaluator                    ();

    virtual void                                    Serialize                               ( ISerializer& sob ) const;
    static OffsetTimeEvaluator *                    Create                                  ( const IDeserializer& dob );

    void                                            SetTimeOffset                           ( TimeType t );

    virtual void                                    SetGlobalTimeImpl                       ( TimeType t ) override;
    virtual TimeType                                GetLocalTime                            () const override;

    virtual const std::string&                      GetType                                 () override;
    static const std::string&                       Type                                    ();
};

DEFINE_PTR_TYPE(OffsetTimeEvaluator)

} //model
} //bv
