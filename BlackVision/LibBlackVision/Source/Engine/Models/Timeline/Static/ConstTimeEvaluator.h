#pragma once

#include "Engine/Models/Timeline/TimeEvaluatorBase.h"

#include "CoreDEF.h"

namespace bv { namespace model {

class ConstTimeEvaluator;
DEFINE_PTR_TYPE(ConstTimeEvaluator)

class ConstTimeEvaluator : public TimeEvaluatorBase< ITimeEvaluator >
{
    FRIEND_TEST_ACCESSOR( ConstTimeEvaluator );
private:

    typedef TimeEvaluatorBase< ITimeEvaluator > Parent;

private:

    TimeType        m_timeVal;

                                    ConstTimeEvaluator  ( const std::string & name, TimeType val );

public:

    static ConstTimeEvaluatorPtr    Create  ( const std::string & name, TimeType val );
                                    ~ConstTimeEvaluator ();

    virtual void                    Serialize           ( ISerializer & sob ) const;
    static ConstTimeEvaluatorPtr    Create              ( const IDeserializer & dob );

    void                            SetConstTimeValue   ( TimeType t );

    virtual void                    SetGlobalTimeImpl   ( TimeType t ) override;
    virtual TimeType                GetLocalTime        () const override;

    virtual const std::string &     GetType             () const override;
    static const std::string &      Type                ();
};

} //model
} //bv
