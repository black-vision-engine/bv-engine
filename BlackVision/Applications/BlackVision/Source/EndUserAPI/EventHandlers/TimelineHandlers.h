#pragma once


#include "Engine/Events/Events.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Interfaces/ITimeline.h"

namespace bv
{
class BVAppLogic;


class TimelineHandlers
{
private:
    BVAppLogic*             m_appLogic;
public:
    TimelineHandlers( BVAppLogic* logic );
    ~TimelineHandlers();

    void        TimelineKeyframe            ( bv::IEventPtr eventPtr );
    void        TimelineHandler             ( bv::IEventPtr evt );

private:
    bool        AddKeyframe     (   TimelineKeyframeEvent::KeyframeType keyframeType,
                                    model::ITimelinePtr timeline,
                                    const std::string& eventName,
                                    TimeType eventTime,
                                    unsigned int totalLoopCount,
                                    TimeType jumpToTime,
                                    std::string & triggerEvents);
};


} //bv
