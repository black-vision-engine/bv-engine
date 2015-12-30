#pragma once


#include "Engine/Events/Events.h"

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

    void        TimelineKeyframe           ( bv::IEventPtr eventPtr );
};


} //bv
