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
void        TimelineHandlers::TimelineHandler     ( bv::IEventPtr evt )
{
    if( evt->GetEventType() == bv::TimeLineEvent::Type() )
    {
		auto editor = m_appLogic->GetBVProject()->GetProjectEditor();

        bv::TimeLineEventPtr timelineEvent = std::static_pointer_cast<bv::TimeLineEvent>( evt );

        std::string & timelinePath = timelineEvent->TimelineName; //path?
        std::string & timelineNewName = timelineEvent->NewTimelineName;

        float time = timelineEvent->Time;
        auto type = timelineEvent->TimelineType;
        auto duration = timelineEvent->Duration;
        auto wrapMethod = timelineEvent->WrapMethod;
        TimeLineEvent::Command command = timelineEvent->TimelineCommand;

        if( command == TimeLineEvent::Command::AddTimeline )
		{
			auto success = editor->AddTimeline( timelinePath, timelineNewName, type );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
		}
		else if( command == TimeLineEvent::Command::DeleteTimeline )
        {
			auto success = editor->DeleteTimeline( timelinePath );
			SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
		else if( command == TimeLineEvent::Command::ForceDeleteTimeline )
        {
			//timelineNewName is more path of new timeline here..
			auto success = editor->ForceDeleteTimeline( timelinePath, timelineNewName );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
		else if( command == TimeLineEvent::Command::RenameTimeline )
        {
			auto success = editor->RenameTimeline( timelinePath, timelineNewName );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
		else if( command == TimeLineEvent::Command::SetDuration )
        {
			auto success = editor->SetTimelineDuration( timelinePath, duration );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
		else if( command == TimeLineEvent::Command::SetWrapPreBehavior )
        {
			auto success = editor->SetTimelineWrapPreBehavior( timelinePath, wrapMethod );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
		else if( command == TimeLineEvent::Command::SetWrapPostBehavior )
        {
			auto success = editor->SetTimelineWrapPostBehavior( timelinePath, wrapMethod );
            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, success );
        }
        else if( command == TimeLineEvent::Command::Play )
        {
            auto timeline = std::static_pointer_cast< model::ITimeline >( editor->GetTimeEvaluator( timelinePath ) );
			if( timeline == nullptr )
            {
                SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Timeline not found" );
            }

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
            timeline->Play();

            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, true );
        }
        else if( command == TimeLineEvent::Command::Stop )
        {
            auto timeline = std::static_pointer_cast< model::ITimeline >( editor->GetTimeEvaluator( timelinePath ) );
			if( timeline == nullptr )
            {
                SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Timeline not found" );
            }

            timeline->Stop();

            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, true );
        }
        else if( command == TimeLineEvent::Command::PlayReverse )
        {
            auto timeline = std::static_pointer_cast< model::ITimeline >( editor->GetTimeEvaluator( timelinePath ) );
			if( timeline == nullptr )
            {
                SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Timeline not found" );
            }

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_BACKWARD );
            timeline->Play();

            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, true );
        }
        else if( command == TimeLineEvent::Command::Goto )
        {
            auto timeline = std::static_pointer_cast< model::ITimeline >( editor->GetTimeEvaluator( timelinePath ) );
			if( timeline == nullptr )
            {
                SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Timeline not found" );
            }

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
            timeline->SetTimeAndStop( ( bv::TimeType )time );

            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, true );
        }
        else if( command == TimeLineEvent::Command::GotoAndPlay )
        {
            auto timeline = std::static_pointer_cast< model::ITimeline >( editor->GetTimeEvaluator( timelinePath ) );
			if( timeline == nullptr )
            {
                SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Timeline not found" );
            }

            timeline->SetPlayDirection( bv::TimelinePlayDirection::TPD_FORWAD );
            timeline->SetTimeAndPlay( ( bv::TimeType )time );

            SendSimpleResponse( command, timelineEvent->EventID, timelineEvent->SocketID, true );
        }
        else
            SendSimpleErrorResponse( command, timelineEvent->EventID, timelineEvent->SocketID, "Unknown command" );
    }
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
