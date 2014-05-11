#pragma once

#include "Engine/Models/Interfaces/ITimeEvaluator.h"

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

    std::string         m_name;

public:

                        TimelineBase    ( TimeType startTime, TimeType endTime, const std::string & name, float scale = 1.f );
                        TimelineBase    ( const TimelineSettings & settings, TimeType startTime, TimeType endTime, const std::string & name, float scale = 1.f );
                        TimelineBase    ( const TimeInterpolator & interpolator, const TimelineSettings & settings, const std::string & name );

    virtual             ~TimelineBase   ();

    void                SetInterval     ( TimeType startTime, TimeType endTime, float scale = 1.f );

    virtual const std::string & GetName () const override;

private:

    TimeType            StartTime       () const;
    TimeType            EndTime         () const;

};

} //model
} //bv
