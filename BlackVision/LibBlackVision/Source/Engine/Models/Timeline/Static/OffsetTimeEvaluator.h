#pragma once

#include <memory>

#include "Engine/Models/Timeline/TimeEvaluatorBase.h"

#include "CoreDEF.h"


namespace bv { namespace model {

class OffsetTimeEvaluator;
DEFINE_PTR_TYPE(OffsetTimeEvaluator)


class OffsetTimeEvaluator : public TimeEvaluatorBase< ITimeEvaluator >
{
    FRIEND_TEST_ACCESSOR( OffsetTimeEvaluator );
private:

    typedef TimeEvaluatorBase< ITimeEvaluator > Parent;

private:

    TimeType        m_timeOffset;
    TimeType        m_timeScale;
    TimeType        m_globalTime;

                                                    OffsetTimeEvaluator                     ( const std::string & name, TimeType offsetTime, TimeType scale );
public:

                                                    ~OffsetTimeEvaluator                    ();

    static OffsetTimeEvaluatorPtr                   Create                                  ( const std::string & name, TimeType offsetTime, TimeType scale = 1 );

    virtual void                                    Serialize                               ( ISerializer& sob ) const;
    static OffsetTimeEvaluatorPtr                   Create                                  ( const IDeserializer& dob );

    void                                            SetTimeOffset                           ( TimeType t );

    virtual void                                    SetGlobalTimeImpl                       ( TimeType t ) override;
    virtual TimeType                                GetLocalTime                            () const override;

    virtual const std::string&                      GetType                                 () const override;
    static const std::string&                       Type                                    ();
};

} //model
} //bv
