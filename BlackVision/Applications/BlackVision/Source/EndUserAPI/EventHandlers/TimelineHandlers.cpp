#include "TimelineHandlers.h"


namespace bv
{

// ***********************
//
TimelineHandlers::TimelineHandlers( BVAppLogic* logic )
        : m_appLogic( logic )
{}

// ***********************
//
TimelineHandlers::~TimelineHandlers()
{}

// ***********************
//
void        TimelineHandlers::TimelineKeyframe           ( bv::IEventPtr eventPtr )
{
    if( eventPtr->GetEventType() != bv::TimelineKeyframeEvent::Type() )
        return;


}

} //bv
