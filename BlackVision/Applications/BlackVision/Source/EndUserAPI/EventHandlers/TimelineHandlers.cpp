#include "stdafxBVApp.h"

#include "TimelineHandlers.h"
#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Models/BVProjectEditor.h"
#include "BVAppLogic.h"


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

    bv::TimelineKeyframeEventPtr keyframeEvent = std::static_pointer_cast<bv::TimelineKeyframeEvent>( eventPtr );

    TimelineKeyframeEvent::Command command  = keyframeEvent->KeyframeCommand;
    TimelineKeyframeEvent::KeyframeType targetType = keyframeEvent->NewKeyframeType;

    std::string& timelinePath   = keyframeEvent->TimelinePath;
    std::string& keyframeName   = keyframeEvent->KeyframeName;
    int keyframeIndex           = keyframeEvent->KeyframeIndex;

    TimeType keyTime            = keyframeEvent->Time;
    TimeType jumpToTime         = keyframeEvent->JumpToTime;
    unsigned int loopCount      = keyframeEvent->TotalLoopCount;
    std::string & triggerEvents = keyframeEvent->TriggerEvents;

    bool keyTimeIsNaN           = mathematics::IsNaN( keyTime );

    auto editor = m_appLogic->GetBVProject()->GetProjectEditor();
    auto timeEvaluator = editor->GetTimeEvaluator( timelinePath );
    if( timeEvaluator == nullptr )
    {
        SendSimpleErrorResponse( command, keyframeEvent->EventID, keyframeEvent->SocketID, "Timeline not found" );
        return;
    }

    if( timeEvaluator->GetType() != model::DefaultTimeline::Type() )
    {
        SendSimpleErrorResponse( command, keyframeEvent->EventID, keyframeEvent->SocketID, "Time evaluator can't be casted to DefaultTimeline" );
        return;
    }
    auto timeline = std::static_pointer_cast< model::DefaultTimeline >( timeEvaluator );

    bool result;
    if( !keyTimeIsNaN && command == TimelineKeyframeEvent::Command::AddKeyframe )
    {
        result = AddKeyframe( targetType, timeline, keyframeName, keyTime, loopCount, jumpToTime, triggerEvents );
        SendSimpleResponse( command, keyframeEvent->EventID, keyframeEvent->SocketID, result );
    }
    else if( command == TimelineKeyframeEvent::Command::RemoveKeyframe )
    {
        if( keyframeIndex > 0 )
            result = timeline->RemoveKeyFrameEvent( keyframeIndex );
        else
            result = timeline->RemoveKeyFrameEvent( keyframeName );
        SendSimpleResponse( command, keyframeEvent->EventID, keyframeEvent->SocketID, result );
    }
    else
        SendSimpleErrorResponse( command, keyframeEvent->EventID, keyframeEvent->SocketID, "Unknown command" );

}


// ***********************
//
void        TimelineHandlers::TimelineHandler     ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::TimeLineEvent::Type() )
    {
		auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

        bv::TimeLineEventPtr timelineEvent = std::static_pointer_cast<bv::TimeLineEvent>( evt );

        std::string & timelinePath = timelineEvent->TimelineName; //path?
        std::string & timelineNewName = timelineEvent->NewTimelineName;

        TimeType time = timelineEvent->Time;
        auto type = timelineEvent->TimelineType;
        auto duration = timelineEvent->Duration;
        auto wrapMethod = timelineEvent->WrapMethod;
        TimeLineEvent::Command command = timelineEvent->TimelineCommand;

        bool timeIsNaN = mathematics::IsNaN( time );

        auto success = false;

        if( command == TimeLineEvent::Command::AddTimeline )
		{
			success = editor->AddTimeline( timelinePath, timelineNewName, type, true );
		}
		else if( command == TimeLineEvent::Command::DeleteTimeline )
        {
			success = editor->DeleteTimeline( timelinePath, true );
        }
		else if( command == TimeLineEvent::Command::ForceDeleteTimeline )
        {
			//timelineNewName is more path of new timeline here..
			success = editor->ForceDeleteTimeline( timelinePath, timelineNewName, true );
        }
		else if( command == TimeLineEvent::Command::RenameTimeline )
        {
			success = editor->RenameTimeline( timelinePath, timelineNewName );
        }
		else if( command == TimeLineEvent::Command::SetDuration )
        {
			success = editor->SetTimelineDuration( timelinePath, duration );
        }
		else if( command == TimeLineEvent::Command::SetWrapPreBehavior )
        {
			success = editor->SetTimelineWrapPreBehavior( timelinePath, wrapMethod );
        }
		else if( command == TimeLineEvent::Command::SetWrapPostBehavior )
        {
			success = editor->SetTimelineWrapPostBehavior( timelinePath, wrapMethod );
        }
        else if( command == TimeLineEvent::Command::Play )
        {
            auto timeline = editor->GetTimeline( timelinePath );
			if( timeline )
            {
                timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
                timeline->Play();
                success = true;
            }
        }
        else if( command == TimeLineEvent::Command::Stop )
        {
            auto timeline = editor->GetTimeline( timelinePath );
			if( timeline )
            {
                timeline->Stop();
                success = true;
            }
        }
        else if( command == TimeLineEvent::Command::PlayReverse )
        {
            auto timeline = editor->GetTimeline( timelinePath );
			if( timeline )
            {
                timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_BACKWARD );
                timeline->Play();
                success = true;
            }
        }
        else if( !timeIsNaN && command == TimeLineEvent::Command::Goto )
        {
            auto timeline = editor->GetTimeline( timelinePath );
			if( timeline )
            {
                timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
                timeline->Play();
                timeline->SetTimeAndStop( time );
                success = true;
            }
        }
        else if( !timeIsNaN && command == TimeLineEvent::Command::GotoAndPlay )
        {
            auto timeline = editor->GetTimeline( timelinePath );
			if( timeline )
            {
                timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
                timeline->SetTimeAndPlay( time );
                success = true;
            }
        }
        else
        {
            SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Unknown command" );
        }


        if( success )
        {
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
        else
        {
            SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, std::string( "Timeline not found [" + timelinePath + "]" ).c_str() );
        }
    }
}

// ***********************
//
bool        TimelineHandlers::AddKeyframe     ( TimelineKeyframeEvent::KeyframeType keyframeType,
                                                model::ITimelinePtr timeline,
                                                const std::string& eventName,
                                                TimeType eventTime,
                                                unsigned int totalLoopCount,
                                                TimeType jumpToTime,
                                                std::string & triggerEvents )
{
    bool result;

    if( keyframeType == TimelineKeyframeEvent::KeyframeType::LoopJumpKeyframe )
        result = model::TimelineManager::GetInstance()->AddLoopJumpEventToTimeline( timeline, eventName, eventTime, totalLoopCount, jumpToTime );
    else if( keyframeType == TimelineKeyframeEvent::KeyframeType::LoopRestartKeyframe )
        result = model::TimelineManager::GetInstance()->AddLoopRestartEventToTimeline( timeline, eventName, eventTime, totalLoopCount );
    else if( keyframeType == TimelineKeyframeEvent::KeyframeType::LoopReverseKeyframe )
        result = model::TimelineManager::GetInstance()->AddLoopReverseEventToTimeline( timeline, eventName, eventTime, totalLoopCount );
    else if( keyframeType == TimelineKeyframeEvent::KeyframeType::NullKeyframe )
        result = model::TimelineManager::GetInstance()->AddNullEventToTimeline( timeline, eventName, eventTime );
    else if( keyframeType == TimelineKeyframeEvent::KeyframeType::StopKeyframe )
        result = model::TimelineManager::GetInstance()->AddStopEventToTimeline( timeline, eventName, eventTime );
    else if( keyframeType == TimelineKeyframeEvent::KeyframeType::TriggerEventKeyframe )
        result = model::TimelineManager::GetInstance()->AddTriggerEventToTimeline( timeline, eventName, eventTime, triggerEvents );
    else
        return false;
    return result;
}

} //bv
