#pragma once

#include "Engine/Models/Timeline/ITimeEvaluator.h"
#include "Engine/Models/Timeline/TimelineSettings.h"
#include "Engine/Models/Timeline/TimelineState.h"
#include "Mathematics/Interpolators/Interpolators.h"


namespace bv { namespace model {

class TimelineBase : public ITimeEvaluator
{
protected:

    TimelineSettings    m_settings;
    TimelineState       m_state;

    TimeInterpolator    m_interpolator;

public:

                        TimelineBase    ( TimeType m_startTime, TimeType m_endTime, float scale = 1.f );
                        TimelineBase    ( const TimelineSettings & settings, TimeType startTime, TimeType endTime, float scale = 1.f );
                        TimelineBase    ( const TimeInterpolator & interpolator, const TimelineSettings & settings );

    virtual             ~TimelineBase   ();

    virtual TimeType    Evaluate        ( TimeType t ) const;

private:

    TimeType            EvalPre         ( TimeType t ) const;
    TimeType            EvalPost        ( TimeType t ) const;

    TimeType            StartTime       () const;
    TimeType            EndTime         () const;

};

} //model
} //bv
