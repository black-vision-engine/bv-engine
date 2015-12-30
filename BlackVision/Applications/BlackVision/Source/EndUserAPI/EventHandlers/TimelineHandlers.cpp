#include "TimelineHandlers.h"
#include "EventHandlerHelpers.h"

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

    bv::TimelineKeyframeEventPtr setParamEvent = std::static_pointer_cast<bv::TimelineKeyframeEvent>( eventPtr );

    TimelineKeyframeEvent::Command command  = setParamEvent->KeyframeCommand;
    //TimelineKeyframeEvent::KeyframeType targetType = setParamEvent->NewKeyframeType;

    //std::string& timelinePath   = setParamEvent->TimelinePath;
    //std::string& keyframeName   = setParamEvent->KeyframeName;

    //float keyTime               = setParamEvent->Time;
    //float jumpToTime            = setParamEvent->JumpToTime;
    //unsigned int loopCount      = setParamEvent->TotalLoopCount;

    if( command == TimelineKeyframeEvent::Command::AddKeyframe )
    {

    }
    else
        SendSimpleErrorResponse( command, setParamEvent->EventID, setParamEvent->SocketID, "Unknown command" );

}

} //bv
