#include "TimelineHandlers.h"
#include "EventHandlerHelpers.h"
#include "Engine/Models/BVProjectEditor.h"
#include "../../BVAppLogic.h"


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
    TimelineKeyframeEvent::KeyframeType targetType = setParamEvent->NewKeyframeType;

    std::string& timelinePath   = setParamEvent->TimelinePath;
    std::string& keyframeName   = setParamEvent->KeyframeName;

    TimeType keyTime            = setParamEvent->Time;
    TimeType jumpToTime         = setParamEvent->JumpToTime;
    unsigned int loopCount      = setParamEvent->TotalLoopCount;

    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();
    auto timeEvaluator = editor->GetTimeEvaluator( timelinePath );
    if( timeEvaluator == nullptr )
    {
        SendSimpleErrorResponse( command, setParamEvent->EventID, setParamEvent->SocketID, "Timeline not found" );
        return;
    }

    if( timeEvaluator->GetType() != DefaultTimeline::Type() )
    {
        SendSimpleErrorResponse( command, setParamEvent->EventID, setParamEvent->SocketID, "Time evaluator can't be casted to DefaultTimeline" );
        return;
    }
    auto timeline = std::static_pointer_cast<DefaultTimeline>( timeEvaluator );

    bool result;
    if( command == TimelineKeyframeEvent::Command::AddKeyframe )
    {
        result = AddKeyframe( targetType, timeline, keyframeName, keyTime, loopCount, jumpToTime );
        SendSimpleResponse( command, setParamEvent->EventID, setParamEvent->SocketID, result );
    }
    else if( command == TimelineKeyframeEvent::Command::RemoveKeyframe )
    {
        result = timeline->RemoveKeyFrameEvent( keyframeName );
        SendSimpleResponse( command, setParamEvent->EventID, setParamEvent->SocketID, result );
    }
    else
        SendSimpleErrorResponse( command, setParamEvent->EventID, setParamEvent->SocketID, "Unknown command" );

}

// ***********************
//
bool        TimelineHandlers::AddKeyframe     ( TimelineKeyframeEvent::KeyframeType keyframeType,
                                                model::ITimelinePtr timeline,
                                                const std::string& eventName,
                                                TimeType eventTime,
                                                unsigned int totalLoopCount,
                                                TimeType jumpToTime )
{
    bool result;

    if( keyframeType == TimelineKeyframeEvent::KeyframeType::LoopJumpKeyframe )
        result = TimelineManager::GetInstance()->AddLoopJumpEventToTimeline( timeline, eventName, eventTime, totalLoopCount, jumpToTime );
    else if( keyframeType == TimelineKeyframeEvent::KeyframeType::LoopRestartKeyframe )
        result = TimelineManager::GetInstance()->AddLoopRestartEventToTimeline( timeline, eventName, eventTime, totalLoopCount );
    else if( keyframeType == TimelineKeyframeEvent::KeyframeType::LoopReverseKeyframe )
        result = TimelineManager::GetInstance()->AddLoopReverseEventToTimeline( timeline, eventName, eventTime, totalLoopCount );
    else if( keyframeType == TimelineKeyframeEvent::KeyframeType::NullKeyframe )
        result = TimelineManager::GetInstance()->AddNullEventToTimeline( timeline, eventName, eventTime );
    else if( keyframeType == TimelineKeyframeEvent::KeyframeType::StopKeyframe )
        result = TimelineManager::GetInstance()->AddStopEventToTimeline( timeline, eventName, eventTime );
    else
        return false;
    return result;
}

} //bv
