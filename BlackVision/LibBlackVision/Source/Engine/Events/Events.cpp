#include "stdafx.h"

#include "Events.h"

#include <cassert>

#pragma warning( push )
#pragma warning(disable:4100)

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "Tools/StringHeplers.h"

#include <limits>



#include "Memory/MemoryLeaks.h"



namespace bv 
{

// ************************************* Static Events' Definitions *************************************

const EventType FrameRenderedEvent::m_sEventType    = EventManager::RegisterType();
std::string FrameRenderedEvent::m_sEventName        = "Event_FrameRendered";

const EventType TransformSetEvent::m_sEventType     = EventManager::RegisterType();
std::string TransformSetEvent::m_sEventName         = "Event_TransformSet";

const EventType KeyPressedEvent::m_sEventType       = EventManager::RegisterType();
std::string KeyPressedEvent::m_sEventName           = "Event_KeyPressedEvent";

const EventType ScreenShotRenderedEvent::m_sEventType       = EventManager::RegisterType();
std::string ScreenShotRenderedEvent::m_sEventName           = "ScreenShotRenderedEvent";


std::string LoadAssetEvent::m_sEventName            = "LoadAssetEvent";
const EventType LoadAssetEvent::m_sEventType        = EventManager::RegisterType( LoadAssetEvent::Name(), LoadAssetEvent::Create );

std::string ParamKeyEvent::m_sEventName             = "ParamKeyEvent";
const EventType ParamKeyEvent::m_sEventType         = EventManager::RegisterType( ParamKeyEvent::Name(), ParamKeyEvent::Create );

std::string NodeStructureEvent::m_sEventName        = "NodeStructureEvent";
const EventType NodeStructureEvent::m_sEventType    = EventManager::RegisterType( NodeStructureEvent::Name(), NodeStructureEvent::Create );

std::string PluginStructureEvent::m_sEventName      = "PluginStructureEvent";
const EventType PluginStructureEvent::m_sEventType  = EventManager::RegisterType( PluginStructureEvent::Name(), PluginStructureEvent::Create );

std::string ProjectEvent::m_sEventName              = "ProjectEvent";
const EventType ProjectEvent::m_sEventType          = EventManager::RegisterType( ProjectEvent::Name(), ProjectEvent::Create );

std::string ResponseEvent::m_sEventName             = "ResponseEvent";
const EventType ResponseEvent::m_sEventType         = EventManager::RegisterType();

std::string InfoEvent::m_sEventName                 = "InfoEvent";
const EventType InfoEvent::m_sEventType             = EventManager::RegisterType( InfoEvent::Name(), InfoEvent::Create );

std::string TimeLineEvent::m_sEventName             = "TimeLineEvent";
const EventType TimeLineEvent::m_sEventType         = EventManager::RegisterType( TimeLineEvent::Name(), TimeLineEvent::Create );

std::string TimerEvent::m_sEventName                = "TimerEvent";
const EventType TimerEvent::m_sEventType            = EventManager::RegisterType( TimerEvent::Name(), TimerEvent::Create );

std::string NodeLogicEvent::m_sEventName            = "NodeLogicEvent";
const EventType NodeLogicEvent::m_sEventType        = EventManager::RegisterType( NodeLogicEvent::Name(), NodeLogicEvent::Create );

std::string VideoCardEvent::m_sEventName            = "VideoCardEvent";
const EventType VideoCardEvent::m_sEventType        = EventManager::RegisterType( VideoCardEvent::Name(), VideoCardEvent::Create );

std::string HightmapEvent::m_sEventName             = "HightmapEvent";
const EventType HightmapEvent::m_sEventType         = EventManager::RegisterType( HightmapEvent::Name(), HightmapEvent::Create );

std::string EngineStateEvent::m_sEventName          = "EngineStateEvent";
const EventType EngineStateEvent::m_sEventType      = EventManager::RegisterType( EngineStateEvent::Name(), EngineStateEvent::Create );

std::string SceneEvent::m_sEventName                = "SceneEvent";
const EventType SceneEvent::m_sEventType            = EventManager::RegisterType( SceneEvent::Name(), SceneEvent::Create );

std::string AssetEvent::m_sEventName                = "AssetEvent";
const EventType AssetEvent::m_sEventType            = EventManager::RegisterType( AssetEvent::Name(), AssetEvent::Create );


std::string GlobalEffectEvent::m_sEventName         = "GlobalEffectEvent";
const EventType GlobalEffectEvent::m_sEventType     = EventManager::RegisterType( GlobalEffectEvent::Name(), GlobalEffectEvent::Create );

std::string TimelineKeyframeEvent::m_sEventName     = "TimelineKeyframeEvent";
const EventType TimelineKeyframeEvent::m_sEventType = EventManager::RegisterType( TimelineKeyframeEvent::Name(), TimelineKeyframeEvent::Create );

std::string MouseEvent::m_sEventName                = "MouseEvent";
const EventType MouseEvent::m_sEventType            = EventManager::RegisterType( MouseEvent::Name(), MouseEvent::Create );

std::string SceneVariableEvent::m_sEventName        = "SceneVariableEvent";
const EventType SceneVariableEvent::m_sEventType    = EventManager::RegisterType( SceneVariableEvent::Name(), SceneVariableEvent::Create );

std::string ConfigEvent::m_sEventName               = "ConfigEvent";
const EventType ConfigEvent::m_sEventType           = EventManager::RegisterType( ConfigEvent::Name(), ConfigEvent::Create );

std::string GridLineEvent::m_sEventName             = "GridLineEvent";
const EventType GridLineEvent::m_sEventType         = EventManager::RegisterType( GridLineEvent::Name(), GridLineEvent::Create );

std::string LightEvent::m_sEventName                = "LightEvent";
const EventType LightEvent::m_sEventType            = EventManager::RegisterType( LightEvent::Name(), LightEvent::Create );

std::string CameraEvent::m_sEventName               = "CameraEvent";
const EventType CameraEvent::m_sEventType           = EventManager::RegisterType( CameraEvent::Name(), CameraEvent::Create );

std::string VideoDecoderEvent::m_sEventName         = "VideoDecoderEvent";
const EventType VideoDecoderEvent::m_sEventType     = EventManager::RegisterType();

std::string ParamDescriptorEvent::m_sEventName      = "ParamDescriptorEvent";
const EventType ParamDescriptorEvent::m_sEventType  = EventManager::RegisterType( ParamDescriptorEvent::Name(), ParamDescriptorEvent::Create );

std::string UndoRedoEvent::m_sEventName             = "UndoRedoEvent";
const EventType UndoRedoEvent::m_sEventType         = EventManager::RegisterType( UndoRedoEvent::Name(), UndoRedoEvent::Create );

std::string AssetTrackerInternalEvent::m_sEventName = "AssetTrackerInternalEvent";
const EventType AssetTrackerInternalEvent::m_sEventType     = EventManager::RegisterType();

std::string GenericEvent::m_sEventName              = "GenericEvent";
const EventType GenericEvent::m_sEventType          = EventManager::RegisterType( GenericEvent::Name(), GenericEvent::Create );

std::string GizmoEvent::m_sEventName                = "GizmoEvent";
const EventType GizmoEvent::m_sEventType            = EventManager::RegisterType( GizmoEvent::Name(), GizmoEvent::Create );




// ************************************* Events Serialization *****************************************


// ========================================================================= //
// ResponseEvent
// ========================================================================= //

namespace SerializationHelper
{
    const std::string RESPONSE_STRING = "Response";
}


// ========================================================================= //
// LoadAssetEvent
// ========================================================================= //

namespace SerializationHelper
{

const std::string ASSET_DATA_STRING = "AssetData";
const std::string ASSET_ASYNC_LOAD = "AsyncLoad";

}

namespace Convert
{

// ***********************
//
std::pair< LoadAssetEvent::Command, const char* > LoadAssetMapping[] =
{ std::make_pair( LoadAssetEvent::Command::LoadAsset, "LoadAsset" )
, std::make_pair( LoadAssetEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( LoadAssetEvent::Command, LoadAssetMapping );

}   // Convert

// ========================================================================= //
// ParamKeyEvent
// ========================================================================= //


namespace SerializationHelper
{

const std::string PARAM_VALUE_STRING = "ParamValue";
const std::string KEY_TIME_STRING = "Time";

}

namespace Convert
{

// ***********************
//
std::pair< ParamKeyEvent::Command, const char* > ParameterCommandMapping[] =
{ std::make_pair( ParamKeyEvent::Command::AddKey, "AddKey" )
, std::make_pair( ParamKeyEvent::Command::RemoveKey, "RemoveKey" )
, std::make_pair( ParamKeyEvent::Command::MoveKey, "MoveKey" )
, std::make_pair( ParamKeyEvent::Command::SetInterpolatorType, "SetInterpolator" )
, std::make_pair( ParamKeyEvent::Command::SetAddedInterpolatorType, "SetAddedInterpolatorType" )
, std::make_pair( ParamKeyEvent::Command::SetInterpolatorPreWrapMethod, "SetInterpolatorPreWrapMethod" )
, std::make_pair( ParamKeyEvent::Command::SetInterpolatorPostWrapMethod, "SetInterpolatorPostWrapMethod" )
, std::make_pair( ParamKeyEvent::Command::AssignTimeline, "AssignTimeline" )
, std::make_pair( ParamKeyEvent::Command::SampleCurve, "SampleCurve" )
, std::make_pair( ParamKeyEvent::Command::ListKeys, "ListKeys" )
, std::make_pair( ParamKeyEvent::Command::ListParameters, "ListParameters" )
, std::make_pair( ParamKeyEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};


IMPLEMENT_ENUM_SERIALIZATION( ParamKeyEvent::Command, ParameterCommandMapping );


}

// ========================================================================= //
// ParamDescriptorEvent
// ========================================================================= //

namespace Convert
{

// ***********************
//
std::pair< ParamDescriptorEvent::Command, const char* > ParamDescriptorCommandMapping[] =
{ std::make_pair( ParamDescriptorEvent::Command::AddParamDescriptor, "AddParamDescriptor" )
    , std::make_pair( ParamDescriptorEvent::Command::RemoveParamDescriptor, "RemoveParamDescriptor" )
    , std::make_pair( ParamDescriptorEvent::Command::AddAvaibleKeyTimes, "AddAvaibleKeyTimes" )
    , std::make_pair( ParamDescriptorEvent::Command::RemoveAvaibleKeyTimes, "RemoveAvaibleKeyTimes" )
    , std::make_pair( ParamDescriptorEvent::Command::SetDescriptorParameters, "SetDescriptorParameters" )
    , std::make_pair( ParamDescriptorEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};


IMPLEMENT_ENUM_SERIALIZATION( ParamDescriptorEvent::Command, ParamDescriptorCommandMapping );

}


// ========================================================================= //
// AssetEvent
// ========================================================================= //

namespace Convert
{

// ***********************
//
std::pair< AssetEvent::Command, const char* > AssetCommandMapping[] =
{
    std::make_pair( AssetEvent::Command::ClearCache, "ClearCache" )
    , std::make_pair( AssetEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( AssetEvent::Command, AssetCommandMapping );

}

// ========================================================================= //
// SceneEvent
// ========================================================================= //

namespace SerializationHelper
{
const std::string NEW_SCENE_NAME_STRING = "NewSceneName";
}


namespace Convert
{

// ***********************
//
std::pair< SceneEvent::Command, const char* > SceneCommandMapping[] =
{
    std::make_pair( SceneEvent::Command::AddScene, "AddScene" )
    , std::make_pair( SceneEvent::Command::RemoveScene, "RemoveScene" )
    , std::make_pair( SceneEvent::Command::RemoveAllScenes, "RemoveAllScenes" )
    , std::make_pair( SceneEvent::Command::SetSceneVisible, "SetSceneVisible" )
    , std::make_pair( SceneEvent::Command::SetSceneInvisible, "SetSceneInvisible" )
    , std::make_pair( SceneEvent::Command::RenameScene, "RenameScene" )
    , std::make_pair( SceneEvent::Command::AttachScene, "AttachScene" )
    , std::make_pair( SceneEvent::Command::DetachScene, "DetachScene" )
    , std::make_pair( SceneEvent::Command::MoveScene, "MoveScene" )
    , std::make_pair( SceneEvent::Command::CopyScene, "CopyScene" )
    , std::make_pair( SceneEvent::Command::SetOutputChannel, "SetOutputChannel" )
    , std::make_pair( SceneEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( SceneEvent::Command, SceneCommandMapping );

}   // Convert

// ========================================================================= //
// LightEvent
// ========================================================================= //

namespace SerializationHelper
{

const std::string LIGHT_TYPE_STRING = "LightType";
const std::string LIGHT_INDEX_STRING = "LightIndex";

}


namespace Convert
{

// ***********************
//
std::pair< LightEvent::Command, const char* > LightCommandMapping[] =
{
    std::make_pair( LightEvent::Command::AddLight, "AddLight" )
    , std::make_pair( LightEvent::Command::RemoveLight, "RemoveLight" )
    , std::make_pair( LightEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( LightEvent::Command, LightCommandMapping );

}

// ========================================================================= //
// CameraEvent
// ========================================================================= //

namespace SerializationHelper
{
    const std::string CAMERA_INDEX_STRING = "CameraIndex";
}

namespace Convert
{

// ***********************
//
std::pair< CameraEvent::Command, const char* > CameraCommandMapping[] =
{
    std::make_pair( CameraEvent::Command::AddCamera, "AddCamera" )
    , std::make_pair( CameraEvent::Command::RemoveCamera, "RemoveCamera" )
    , std::make_pair( CameraEvent::Command::SetCurrentCamera, "SetCurrentCamera" )
    , std::make_pair( CameraEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( CameraEvent::Command, CameraCommandMapping );

}


// ========================================================================= //
// NodeStructureEvent
// ========================================================================= //


namespace SerializationHelper
{
   const std::string NEW_NODE_NAME_STRING = "NewNodeName";
}

namespace Convert
{

// ***********************
//
std::pair< NodeStructureEvent::Command, const char* > NodeStructureCommandMapping[] =
{
    std::make_pair( NodeStructureEvent::Command::AddNode, "AddNode" )
    , std::make_pair( NodeStructureEvent::Command::RemoveNode, "RemoveNode" )
    , std::make_pair( NodeStructureEvent::Command::SetNodeInvisible, "SetNodeInvisible" )
    , std::make_pair( NodeStructureEvent::Command::SetNodeVisible, "SetNodeVisible" )
    , std::make_pair( NodeStructureEvent::Command::SelectNode, "SelectNode" )
    , std::make_pair( NodeStructureEvent::Command::UnselectNodes, "UnselectNodes" )
    , std::make_pair( NodeStructureEvent::Command::RenameNode, "RenameNode" )
    , std::make_pair( NodeStructureEvent::Command::AttachNode, "AttachNode" )
    , std::make_pair( NodeStructureEvent::Command::DetachNode, "DetachNode" )
    , std::make_pair( NodeStructureEvent::Command::MoveNode, "MoveNode" )
    , std::make_pair( NodeStructureEvent::Command::CopyNode, "CopyNode" )
    , std::make_pair( NodeStructureEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( NodeStructureEvent::Command, NodeStructureCommandMapping );

}

// ========================================================================= //
// PluginStructureEvent
// ========================================================================= //

namespace SerializationHelper
{
    const std::string ATTACH_INDEX_STRING = "AttachIndex";
    const std::string PLUGIN_UID_STRING = "PluginUID";
}


namespace Convert
{

// ***********************
//
std::pair< PluginStructureEvent::Command, const char* > PluginStructureCommandMapping[] =
{
    std::make_pair( PluginStructureEvent::Command::AttachPlugin, "AttachPlugin" )
    , std::make_pair( PluginStructureEvent::Command::DetachPlugin, "DetachPlugin" )
    , std::make_pair( PluginStructureEvent::Command::AddPlugin, "AddPlugin" )
    , std::make_pair( PluginStructureEvent::Command::RemovePlugin, "RemovePlugin" )
    , std::make_pair( PluginStructureEvent::Command::CopyPlugin, "CopyPlugin" )
    , std::make_pair( PluginStructureEvent::Command::MovePlugin, "MovePlugin" )
    , std::make_pair( PluginStructureEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( PluginStructureEvent::Command, PluginStructureCommandMapping );

}

// ========================================================================= //
// ProjectEvent
// ========================================================================= //

namespace Convert
{

// ***********************
//
std::pair< ProjectEvent::Command, const char* > ProjectEventCommandMapping[] =
{
    std::make_pair( ProjectEvent::Command::SaveScene, "SaveScene" )
    , std::make_pair( ProjectEvent::Command::LoadScene, "LoadScene" )
    , std::make_pair( ProjectEvent::Command::RemoveScene, "RemoveScene" )
    , std::make_pair( ProjectEvent::Command::CopyScene, "CopyScene" )
    , std::make_pair( ProjectEvent::Command::MoveScene, "MoveScene" )

    , std::make_pair( ProjectEvent::Command::AddExistingSceneToProject, "AddExistingSceneToProject" )
    , std::make_pair( ProjectEvent::Command::CreateSceneInProject, "CreateSceneInProject" )

    , std::make_pair( ProjectEvent::Command::LoadProject, "LoadProject" )
    , std::make_pair( ProjectEvent::Command::NewProject, "NewProject" )
    , std::make_pair( ProjectEvent::Command::SetCurrentProject, "SetCurrentProject" )
    , std::make_pair( ProjectEvent::Command::MoveProject, "MoveProject" )
    , std::make_pair( ProjectEvent::Command::DeleteProject, "DeleteProject" )
    , std::make_pair( ProjectEvent::Command::RenameProject, "RenameProject" )

    , std::make_pair( ProjectEvent::Command::CreateFolder, "CreateFolder" )
    , std::make_pair( ProjectEvent::Command::DeleteFolder, "DeleteFolder" )
    , std::make_pair( ProjectEvent::Command::RenameFolder, "RenameFolder" )
    , std::make_pair( ProjectEvent::Command::CopyFolder, "CopyFolder" )
    , std::make_pair( ProjectEvent::Command::MoveFolder, "MoveFolder" )

    , std::make_pair( ProjectEvent::Command::CopyAsset, "CopyAsset" )
    , std::make_pair( ProjectEvent::Command::MoveAsset, "MoveAsset" )
    , std::make_pair( ProjectEvent::Command::RemoveAsset, "RemoveAsset" )
    , std::make_pair( ProjectEvent::Command::ImportAsset, "ImportAsset" )

    , std::make_pair( ProjectEvent::Command::SavePreset, "SavePreset" )
    , std::make_pair( ProjectEvent::Command::LoadPreset, "LoadPreset" )
    , std::make_pair( ProjectEvent::Command::EditPreset, "EditPreset" )

    , std::make_pair( ProjectEvent::Command::GenerateMeshThumbnail, "GenerateMeshThumbnail" ) //temp testing event

    , std::make_pair( ProjectEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( ProjectEvent::Command, ProjectEventCommandMapping );

}

// ========================================================================= //
// InfoEvent
// ========================================================================= //

namespace Convert
{

// ***********************
//
std::pair< InfoEvent::Command, const char* > InfoEventCommandMapping[] =
{
    std::make_pair( InfoEvent::Command::TreeStructure, "TreeStructure" )
    , std::make_pair( InfoEvent::Command::MinimalTreeStructure, "MinimalTreeStructure" )
    , std::make_pair( InfoEvent::Command::NodeInfo, "NodeInfo" )
    , std::make_pair( InfoEvent::Command::PluginInfo, "PluginInfo" )
    , std::make_pair( InfoEvent::Command::EffectInfo, "EffectInfo" )
    , std::make_pair( InfoEvent::Command::ParamInfo, "ParamInfo" )
    , std::make_pair( InfoEvent::Command::LogicInfo, "LogicInfo" )
    , std::make_pair( InfoEvent::Command::MinimalSceneInfo, "MinimalSceneInfo" )
    , std::make_pair( InfoEvent::Command::LightsInfo, "LightsInfo" )
    , std::make_pair( InfoEvent::Command::CamerasInfo, "CamerasInfo" )
    , std::make_pair( InfoEvent::Command::ListParamDescriptors, "ListParamDescriptors" )
    , std::make_pair( InfoEvent::Command::GetParamDescriptor, "GetParamDescriptor" )

    , std::make_pair( InfoEvent::Command::ListSceneAssets, "ListSceneAssets" )
    , std::make_pair( InfoEvent::Command::ListProjectNames, "ListProjectNames" )
    , std::make_pair( InfoEvent::Command::ListScenes, "ListScenes" )
    , std::make_pair( InfoEvent::Command::ListPresets, "ListPresets" )
    , std::make_pair( InfoEvent::Command::ListAssetsPaths, "ListAssetsPaths" )
    , std::make_pair( InfoEvent::Command::ListCategoriesNames, "ListCategoriesNames" )
    , std::make_pair( InfoEvent::Command::ListProjects, "ListProjects" )
    , std::make_pair( InfoEvent::Command::ListAllScenes, "ListAllScenes" )
    , std::make_pair( InfoEvent::Command::ListAllFolders, "ListAllFolders" )
    , std::make_pair( InfoEvent::Command::ListResourcesInFolders, "ListResourcesInFolders" )
    , std::make_pair( InfoEvent::Command::ListAllResources, "ListAllResources" )
    , std::make_pair( InfoEvent::Command::GetAssetDescriptor, "GetAssetDescriptor" )
    , std::make_pair( InfoEvent::Command::GetAssetThumbnail, "GetAssetThumbnail" )
    , std::make_pair( InfoEvent::Command::GetSceneThumbnail, "GetSceneThumbnail" )
    , std::make_pair( InfoEvent::Command::GetPresetThumbnail, "GetPresetThumbnail" )

    , std::make_pair( InfoEvent::Command::GetPMItemStats, "GetPMItemStats" )

    , std::make_pair( InfoEvent::Command::Timelines, "TimeLines" )
    , std::make_pair( InfoEvent::Command::ListTimelineKeyframes, "ListTimelineKeyframes" )
    , std::make_pair( InfoEvent::Command::CheckTimelineTime, "CheckTimelineTime" )
    , std::make_pair( InfoEvent::Command::ListTimelinesParams, "ListTimelinesParams" )

    , std::make_pair( InfoEvent::Command::Performance, "Performance" )
    , std::make_pair( InfoEvent::Command::Videocards, "VideoCards" )
    , std::make_pair( InfoEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( InfoEvent::Command, InfoEventCommandMapping );

}

// ========================================================================= //
// TimeLineEvent
// ========================================================================= //


namespace SerializationHelper
{
    const std::string TIMELINE_TYPE_STRING = "Type";
    const std::string TIMELINE_TIME_VALUE_STRING = "Time";
    const std::string TIMELINE_DURATION_VALUE_STRING = "Duration";
    const std::string TIMELINE_NEW_NAME_STRING = "TimelineNewName";
    const std::string TIMELINE_WRAP_METHOD_STRING = "WrapMethod";
}

namespace Convert
{

// ***********************
//
std::pair< TimeLineEvent::Command, const char* > TimeLineEventCommandMapping[] =
{
    std::make_pair( TimeLineEvent::Command::AddTimeline, "AddTimeline" )
    , std::make_pair( TimeLineEvent::Command::DeleteTimeline, "DeleteTimeline" )
    , std::make_pair( TimeLineEvent::Command::ForceDeleteTimeline, "ForceDeleteTimeline" )
    , std::make_pair( TimeLineEvent::Command::RenameTimeline, "RenameTimeline" )
    , std::make_pair( TimeLineEvent::Command::SetDuration, "SetDuration" )
    , std::make_pair( TimeLineEvent::Command::SetWrapPreBehavior, "SetWrapPreBehavior" )
    , std::make_pair( TimeLineEvent::Command::SetWrapPostBehavior, "SetWrapPostBehavior" )
    , std::make_pair( TimeLineEvent::Command::Play, "Play" )
    , std::make_pair( TimeLineEvent::Command::Stop, "Stop" )
    , std::make_pair( TimeLineEvent::Command::PlayReverse, "PlayReverse" )
    , std::make_pair( TimeLineEvent::Command::Goto, "Goto" )
    , std::make_pair( TimeLineEvent::Command::GotoAndPlay, "GotoAndPlay" )
    , std::make_pair( TimeLineEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( TimeLineEvent::Command, TimeLineEventCommandMapping );

// ***********************
//
std::pair< TimelineType, const char* > TimelineTypeMapping[] =
{
    std::make_pair( TimelineType::TT_DEFAULT, "Default" )
    , std::make_pair( TimelineType::TT_OFFSET, "Offset" )
    , std::make_pair( TimelineType::TT_CONST, "Const" )
    , std::make_pair( TimelineType::TT_TOTAL, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( TimelineType, TimelineTypeMapping );

}   // Convert



// ========================================================================= //
// NodeLogicEvent
// ========================================================================= //

namespace SerializationHelper
{
    const std::string WIDGET_ACTION_STRING = "Action";
    const std::string WIDGET_TIME_VALUE_STRING = "Time";
}

namespace Convert
{

// ***********************
//
std::pair< NodeLogicEvent::Command, const char* > NodeLogicEventCommandMapping[] =
{
    std::make_pair( NodeLogicEvent::Command::AddNodeLogic, "AddNodeLogic" )
    , std::make_pair( NodeLogicEvent::Command::DeleteNodeLogic, "DeleteNodeLogic" )
    , std::make_pair( NodeLogicEvent::Command::LogicAction, "LogicAction" )
    , std::make_pair( NodeLogicEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( NodeLogicEvent::Command, NodeLogicEventCommandMapping );

}


// ========================================================================= //
// VideoCardEvent
// ========================================================================= //

namespace SerializationHelper
{
    const std::string VIDEO_CARD_ID_STRING = "VideoCardID";
    const std::string VIDEO_CARD_CHANNEL_ID_STRING = "ChannelID";
    const std::string VIDEO_CARD_NUMBER_STRING = "Number";
    const std::string VIDEO_CARD_VALUE_STRING = "Value";
    const std::string VIDEO_CARD_REFERENCE_MODE_STRING = "ReferenceMode";
}


namespace Convert
{

// ***********************
//
std::pair< VideoCardEvent::Command, const char* > VideoCardEventCommandMapping[] =
{
    std::make_pair( VideoCardEvent::Command::EnableOutput, "EnableOutput" )
    , std::make_pair( VideoCardEvent::Command::DisableOutput, "DisableOutput" )
    , std::make_pair( VideoCardEvent::Command::EnableKey, "EnableKey" )
    , std::make_pair( VideoCardEvent::Command::DisableKey, "DisableKey" )
    , std::make_pair( VideoCardEvent::Command::SetReferenceMode, "SetReferenceMode" )
    , std::make_pair( VideoCardEvent::Command::SetReferenceOffsetH, "SetReferenceOffsetH" )
    , std::make_pair( VideoCardEvent::Command::SetReferenceOffsetV, "SetReferenceOffsetV" )
    , std::make_pair( VideoCardEvent::Command::EnableInput, "EnableInput" )
    , std::make_pair( VideoCardEvent::Command::DisableInput, "DisableInput" )
    , std::make_pair( VideoCardEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( VideoCardEvent::Command, VideoCardEventCommandMapping );


}   // Convert


// ========================================================================= //
// TimerEvent
// ========================================================================= //


namespace SerializationHelper
{
    const std::string TIMER_HOURS_STRING = "Hours";
    const std::string TIMER_MINUTES_STRING = "Minutes";
    const std::string TIMER_SECONDS_STRING = "Seconds";
    const std::string TIMER_MILLISECONDS_STRING = "Milliseconds";
    const std::string TIMER_TIME_PATERN_STRING = "TimePatern";
}

namespace Convert
{

// ***********************
//
std::pair< TimerEvent::Command, const char* > TimerEventCommandMapping[] =
{
    std::make_pair( TimerEvent::Command::Start, "Start" )
    , std::make_pair( TimerEvent::Command::Stop, "Stop" )
    , std::make_pair( TimerEvent::Command::Reset, "Reset" )
    , std::make_pair( TimerEvent::Command::SetTime, "SetTime" )
    , std::make_pair( TimerEvent::Command::SetTimeStart, "SetTimeStart" )
    , std::make_pair( TimerEvent::Command::SetTimeStop, "SetTimeStop" )
    , std::make_pair( TimerEvent::Command::SetTimePatern, "SetTimePatern" )
    , std::make_pair( TimerEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( TimerEvent::Command, TimerEventCommandMapping );

}


// ========================================================================= //
// VideoDecoderEvent
// ========================================================================= //

namespace SerializationHelper
{
    const std::string ASSET_PATH_STRING = "AssetPath";
}


namespace Convert
{

// ***********************
//
std::pair< VideoDecoderEvent::Command, const char* > VideoDecoderEventMapping[] =
{
    std::make_pair( VideoDecoderEvent::Command::HasFinished, "HasFinished" )
    , std::make_pair( VideoDecoderEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( VideoDecoderEvent::Command, VideoDecoderEventMapping );

}



// ========================================================================= //
// EngineStateEvent
// ========================================================================= //


namespace SerializationHelper
{
    const std::string REQUESTED_FPS_STRING = "FPS";
    const std::string NUM_FRAMES_STRING = "NumberFrames";
    const std::string RENDERING_FILE_PATH = "FilePath";
    const std::string GAIN_VAL_STRING = "Gain";
}


namespace Convert
{

// ***********************
//
std::pair< EngineStateEvent::Command, const char* > EngineStateEventCommandMapping[] =
{
    std::make_pair( EngineStateEvent::Command::ScreenShot, "ScreenShot" )
    , std::make_pair( EngineStateEvent::Command::RenderOffscreen, "RenderOffscreen" )
    , std::make_pair( EngineStateEvent::Command::CloseApplication, "CloseApplication" )
    , std::make_pair( EngineStateEvent::Command::LockEventQueue, "LockEventQueue" )
    , std::make_pair( EngineStateEvent::Command::SetGain, "SetGain" )
    , std::make_pair( EngineStateEvent::Command::OutputCommand, "OutputCommand" )
    , std::make_pair( EngineStateEvent::Command::SwitchEditMode, "SwitchEditMode" )
    , std::make_pair( EngineStateEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( EngineStateEvent::Command, EngineStateEventCommandMapping );

}

// ========================================================================= //
// GlobalEffectEvent
// ========================================================================= //


namespace SerializationHelper
{
    const std::string GLOBAL_EFFECT_NAME_STRING = "GlobalEffectName";
}

namespace Convert
{

// ***********************
//
std::pair< GlobalEffectEvent::Command, const char* > GlobalEffectEventCommandMapping[] =
{
    std::make_pair( GlobalEffectEvent::Command::SetGlobalEffect, "SetGlobalEffect" )
    , std::make_pair( GlobalEffectEvent::Command::LoadGlobalEffectAsset, "LoadGlobalEffectAsset" )
    , std::make_pair( GlobalEffectEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( GlobalEffectEvent::Command, GlobalEffectEventCommandMapping );
}



// ========================================================================= //
// TimelineKeyframeEvent
// ========================================================================= //


namespace SerializationHelper
{
    const std::string KEYFRAME_NAME_STRING = "KeyframeName";
    const std::string KEYFRAME_INDEX_STRING = "KeyframeIndex";
    const std::string NEW_KEYFRAME_TYPE_STRING = "NewKeyframeType";
    const std::string JUMP_TO_TIME_STRING = "JumpToTime";
    const std::string TOTAL_LOOP_COUNT_STRING = "TotalLoopCount";
    const std::string TRIGGER_EVENTS = "TriggerEvents";
}


namespace Convert
{

// ***********************
//
std::pair< TimelineKeyframeEvent::Command, const char* > TimelineKeyframeEventCommandMapping[] =
{
    std::make_pair( TimelineKeyframeEvent::Command::AddKeyframe, "AddKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::Command::RemoveKeyframe, "RemoveKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( TimelineKeyframeEvent::Command, TimelineKeyframeEventCommandMapping );

}

namespace Convert
{

// ***********************
//
std::pair< TimelineKeyframeEvent::KeyframeType, const char* > KeyframeTypeMapping[] =
{
    std::make_pair( TimelineKeyframeEvent::KeyframeType::StopKeyframe, "StopKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::LoopReverseKeyframe, "LoopReverseKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::LoopJumpKeyframe, "LoopJumpKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::LoopRestartKeyframe, "LoopRestartKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::TriggerEventKeyframe, "TriggerEventKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::NullKeyframe, "NullKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::KeyframeTypeFail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( TimelineKeyframeEvent::KeyframeType, KeyframeTypeMapping );

}

// ========================================================================= //
// MouseEvent
// ========================================================================= //

namespace SerializationHelper
{
    const std::string MOUSE_X_STRING = "MouseX";
    const std::string MOUSE_Y_STRING = "MouseY";
    const std::string MOUSE_AUTO_SELECT_FLAG = "AutoSelect";
    const std::string MOUSE_AUTO_SELECT_COLOR = "AutoSelectColor";
}

namespace Convert
{

// ***********************
//
std::pair< MouseEvent::Command, const char* > MouseEventCommandMapping[] =
{
    std::make_pair( MouseEvent::Command::MouseUp, "MouseUp" )
    , std::make_pair( MouseEvent::Command::MouseDown, "MouseDown" )
    , std::make_pair( MouseEvent::Command::MouseMove, "MouseMove" )
    , std::make_pair( MouseEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( MouseEvent::Command, MouseEventCommandMapping );

}


// ========================================================================= //
// SceneVariableEvent
// ========================================================================= //

namespace SerializationHelper
{
    const std::string SCENE_VARIABLE_NAME_STRING = "VariableName";
    const std::string SCENE_VARAIBLE_CONTENT_STRING = "VariableContent";
}

namespace Convert
{

// ***********************
//
std::pair< SceneVariableEvent::Command, const char* > SceneVariableEventCommandMapping[] =
{
    std::make_pair( SceneVariableEvent::Command::AddVariable, "AddVariable" )
    , std::make_pair( SceneVariableEvent::Command::GetVariable, "GetVariable" )
    , std::make_pair( SceneVariableEvent::Command::DeleteVariable, "DeleteVariable" )
    , std::make_pair( SceneVariableEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( SceneVariableEvent::Command, SceneVariableEventCommandMapping );

}

// ========================================================================= //
// ConfigEvent
// ========================================================================= //


namespace SerializationHelper
{
    const std::string CONFIG_VALUE_STRING = "Value";
    const std::string CONFIG_KEY_STRING = "Key";
}

namespace Convert
{

// ***********************
//
std::pair< ConfigEvent::Command, const char* > ConfigEventCommandMapping[] =
{
    std::make_pair( ConfigEvent::Command::ReadValue, "ReadValue" )
    , std::make_pair( ConfigEvent::Command::SetValue, "SetValue" )
    , std::make_pair( ConfigEvent::Command::SaveConfig, "SaveConfig" )
    , std::make_pair( ConfigEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( ConfigEvent::Command, ConfigEventCommandMapping );

}


// ========================================================================= //
// GridLineEvent
// ========================================================================= //


namespace SerializationHelper
{
    const std::string GRIDLINE_NAME_STRING = "GridLineName";
    const std::string GRIDLINE_POSITION_STRING = "GridLinePosition";
    const std::string GRIDLINE_TYPE_STRING = "GridLineType";
    const std::string GRIDLINE_INDEX_STRING = "GridLineIndex";
    const std::string GRIDLINE_ALIGNEMENT_TYPE_STRING = "AlignementType";
}

namespace Convert
{

// ***********************
//
std::pair< GridLineEvent::Command, const char* > GridLineEventEventCommandMapping[] =
{
    std::make_pair( GridLineEvent::Command::SetGridLinePosition, "SetGridLinePosition" )
    , std::make_pair( GridLineEvent::Command::RenameGridLine, "RenameGridLine" )
    , std::make_pair( GridLineEvent::Command::RemoveGridLine, "RemoveGridLine" )
    , std::make_pair( GridLineEvent::Command::AlignToGridLine, "AlignToGridLine" )
    , std::make_pair( GridLineEvent::Command::ShowGridLines, "ShowGridLines" )
    , std::make_pair( GridLineEvent::Command::HideGridLines, "HideGridLines" )
    , std::make_pair( GridLineEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( GridLineEvent::Command, GridLineEventEventCommandMapping );

}



// ========================================================================= //
// UndoRedoEvent
// ========================================================================= //


namespace SerializationHelper
{
    const std::string UNDO_REDO_NUM_STEPS_STRING = "Steps";
    const std::string UNDO_REDO_OPERATION_BUFF_SIZE_STRING = "MaxOperations";
}

namespace Convert
{

// ***********************
//
std::pair< UndoRedoEvent::Command, const char* > UndoRedoEventCommandMapping[] =
{
    std::make_pair( UndoRedoEvent::Command::Redo, "Redo" )
    , std::make_pair( UndoRedoEvent::Command::SetOperationsBufferSize, "SetOperationsBufferSize" )
    , std::make_pair( UndoRedoEvent::Command::Undo, "Undo" )
    , std::make_pair( UndoRedoEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( UndoRedoEvent::Command, UndoRedoEventCommandMapping );

}

// ========================================================================= //
// HightmapEvent
// ========================================================================= //

namespace SerializationHelper
{
    const std::string COMMAND_HM_ENABLE_STRING = "Enable";
    const std::string COMMAND_HM_START_STRING = "Start";
    const std::string COMMAND_HM_RESET_STRING = "Reset";
    const std::string COMMAND_HM_SHOW_STRING = "Show";
    const std::string COMMAND_HM_ZOOM_STRING = "Zoom";
    const std::string COMMAND_HM_ZOOM_OUT_STRING = "ZoomOut";
    const std::string COMMAND_HM_SET_STRING = "Set";
    const std::string COMMAND_HM_ANIM_STRING = "Anim";
    const std::string COMMAND_HM_ANIM2_STRING = "Anim2";
    const std::string COMMAND_HM_SET3_STRING = "Set3";
}

namespace Convert
{

// ***********************
//
std::pair< HightmapEvent::Command, const char* > HightmapEventCommandMapping[] =
{
    std::make_pair( HightmapEvent::Command::Enable, "Enable" )
    , std::make_pair( HightmapEvent::Command::Start, "Start" )
    , std::make_pair( HightmapEvent::Command::Reset, "Reset" )
    , std::make_pair( HightmapEvent::Command::Show, "Show" )
    , std::make_pair( HightmapEvent::Command::Zoom, "Zoom" )
    , std::make_pair( HightmapEvent::Command::ZoomOut, "ZoomOut" )
    , std::make_pair( HightmapEvent::Command::Set, "Set" )
    , std::make_pair( HightmapEvent::Command::Anim, "Anim" )
    , std::make_pair( HightmapEvent::Command::Anim2, "Anim2" )
    , std::make_pair( HightmapEvent::Command::Set3, "Set3" )
    , std::make_pair( HightmapEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};
IMPLEMENT_ENUM_SERIALIZATION( HightmapEvent::Command, HightmapEventCommandMapping );

}

// ========================================================================= //
// GizmoEvent
// ========================================================================= //

namespace Convert
{

// ***********************
//
std::pair< GizmoEvent::Command, const char* > GizmoEventCommandMapping[] =
{
    std::make_pair( GizmoEvent::Command::CreateGizmo, "CreateGizmo" )
    , std::make_pair( GizmoEvent::Command::RemoveGizmo, "RemoveGizmo" )      // default
    , std::make_pair( GizmoEvent::Command::ListGizmos, "ListGizmos" )      // default
    , std::make_pair( GizmoEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

IMPLEMENT_ENUM_SERIALIZATION( GizmoEvent::Command, GizmoEventCommandMapping );

}


// ************************************* FrameRenderedEvent *************************************

// *************************************
//
FrameRenderedEvent::FrameRenderedEvent                          ( bv::Renderer * renderer )
    : m_renderer( renderer )
{}
// *************************************
//
void                FrameRenderedEvent::Serialize            ( ISerializer& ser ) const
{    assert( !"Should not be serialized" );  }

// *************************************
//
IEventPtr                FrameRenderedEvent::Create          ( IDeserializer& deser )
{
    assert( !"Should not be deserialized" );
    return nullptr;    
}
// *************************************
//
IEventPtr               FrameRenderedEvent::Clone             () const
{   return IEventPtr( new FrameRenderedEvent( *this ) );  }

// *************************************
//
EventType           FrameRenderedEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        FrameRenderedEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  FrameRenderedEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           FrameRenderedEvent::GetEventType() const
{   return this->m_sEventType; }

// *************************************
//
Renderer *              FrameRenderedEvent::Renderer            ()
{    return m_renderer; }

// *************************************
//
void                    FrameRenderedEvent::SetResolution       ( int w, int h )
{
    m_width = w;
    m_height = h;
}

// *************************************
//
int                     FrameRenderedEvent::Width               () const
{    return m_width;    }

// *************************************
//
int                     FrameRenderedEvent::Height              () const
{    return m_height;   }



// ************************************* TransformSetEvent *************************************

// *************************************
//
TransformSetEvent::TransformSetEvent                        ()
    : m_translation( 0.f, 0.f, 0.f )
    , m_scale( 1.f, 1.f, 1.f )
{}

// *************************************
//
void                TransformSetEvent::Serialize            ( ISerializer& ser ) const
{    assert( !"Should not be serialized" );  }

// *************************************
//
IEventPtr                TransformSetEvent::Create          ( IDeserializer& deser )
{
    assert( !"Should not be deserialized" );
    return nullptr;    
}
// *************************************
//
IEventPtr               TransformSetEvent::Clone             () const
{   return IEventPtr( new TransformSetEvent( *this ) );  }

// *************************************
//
EventType           TransformSetEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        TransformSetEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  TransformSetEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           TransformSetEvent::GetEventType() const
{   return this->m_sEventType; }

// *************************************
//
void                TransformSetEvent::SetTranslation       ( const glm::vec3 & translation )
{    m_translation = translation;   }

// *************************************
//
void                TransformSetEvent::SetScale             ( const glm::vec3 & scale )
{    m_scale = scale;   }

// *************************************
//
const glm::vec3 &   TransformSetEvent::Translation          () const
{    return m_translation;  }

// *************************************
//
const glm::vec3 &   TransformSetEvent::Scale                () const
{    return m_scale;    }


// ************************************* KeyPressedEvent *************************************


// *************************************
//
KeyPressedEvent::KeyPressedEvent        ()
{}

// *************************************
//
void                KeyPressedEvent::Serialize            ( ISerializer& ser ) const
{    assert( !"Should not be serialized" );  }

// *************************************
//
IEventPtr                KeyPressedEvent::Create          ( IDeserializer& deser )
{
    assert( !"Should not be deserialized" );
    return nullptr;    
}
// *************************************
//
IEventPtr               KeyPressedEvent::Clone             () const
{   return IEventPtr( new KeyPressedEvent( *this ) );  }

// *************************************
//
EventType           KeyPressedEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        KeyPressedEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  KeyPressedEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           KeyPressedEvent::GetEventType() const
{   return this->m_sEventType; }

// *************************************
//
void                    KeyPressedEvent::SetChar             ( unsigned char  c )
{    m_char = c;    }

// *************************************
//
unsigned char           KeyPressedEvent::GetChar             () const
{    return m_char; }


// ************************************* ScreenShotRenderedEvent *************************************


// *************************************
//
ScreenShotRenderedEvent::ScreenShotRenderedEvent        ()
{}

// *************************************
//
void                ScreenShotRenderedEvent::Serialize            ( ISerializer& ser ) const
{    assert( !"Should not be serialized" );  }

// *************************************
//
IEventPtr                ScreenShotRenderedEvent::Create          ( IDeserializer& deser )
{
    assert( !"Should not be deserialized" );
    return nullptr;    
}
// *************************************
//
IEventPtr               ScreenShotRenderedEvent::Clone             () const
{   return IEventPtr( new ScreenShotRenderedEvent( *this ) );  }

// *************************************
//
EventType           ScreenShotRenderedEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        ScreenShotRenderedEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  ScreenShotRenderedEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           ScreenShotRenderedEvent::GetEventType() const
{   return this->m_sEventType; }

//******************* LoadAssetEvent *************

// *************************************
//
void                LoadAssetEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodeName );
    ser.SetAttribute( SerializationHelper::PLUGIN_NAME_STRING, PluginName );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::ASSET_ASYNC_LOAD, Convert::T2String( AsyncLoad ) );

    ser.EnterChild( SerializationHelper::ASSET_DATA_STRING );
    ser.ExitChild();
}

// *************************************
//
IEventPtr                LoadAssetEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        LoadAssetEventPtr newEvent  = std::make_shared<LoadAssetEvent>();
        newEvent->PluginName        = deser.GetAttribute( SerializationHelper::PLUGIN_NAME_STRING );
        newEvent->NodeName          = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->AssetData         = deser.DetachBranch( SerializationHelper::ASSET_DATA_STRING );
        newEvent->AsyncLoad         = Convert::String2T< bool >( deser.GetAttribute( SerializationHelper::ASSET_ASYNC_LOAD ), false );
        return newEvent;
    }
    return nullptr;
}
// *************************************
//
IEventPtr               LoadAssetEvent::Clone             () const
{    return IEventPtr( new LoadAssetEvent( *this ) );   }

// *************************************
//
EventType               LoadAssetEvent::Type              ()
{    return m_sEventType;   }
// *************************************
//
std::string& LoadAssetEvent::Name                ()
{    return m_sEventName;   }
// *************************************
//
const std::string &     LoadAssetEvent::GetName           () const
{    return Name();   }
// *************************************
//
EventType           LoadAssetEvent::GetEventType         () const
{    return this->m_sEventType; }


//******************* ParamKeyEvent *************

// *************************************
//
void                ParamKeyEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );

    ParamAddress.Serialize( ser );

    ser.SetAttribute( SerializationHelper::PARAM_VALUE_STRING, Value );
    ser.SetAttribute( SerializationHelper::KEY_TIME_STRING, Convert::T2String( Time ) );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( ParamCommand ) );
}

// *************************************
//
IEventPtr           ParamKeyEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        ParamKeyEventPtr newEvent   = std::make_shared<ParamKeyEvent>();

        newEvent->ParamAddress      = ParameterAddress::Create( deser );

        newEvent->Value             = deser.GetAttribute( SerializationHelper::PARAM_VALUE_STRING );
        newEvent->ParamCommand      = Convert::String2T<ParamKeyEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), ParamKeyEvent::Command::Fail );        
        newEvent->Time              = Convert::String2T< TimeType >( deser.GetAttribute( SerializationHelper::KEY_TIME_STRING ), std::numeric_limits< TimeType >::quiet_NaN() );

        return newEvent;
    }
    else
        return nullptr;    
}

// *************************************
//
IEventPtr               ParamKeyEvent::Clone             () const
{ return IEventPtr( new ParamKeyEvent( *this ) ); }



// *************************************
//
EventType               ParamKeyEvent::Type              ()
{    return m_sEventType;   }
// *************************************
//
std::string& ParamKeyEvent::Name                ()
{    return m_sEventName;   }
// *************************************
//
const std::string &     ParamKeyEvent::GetName           () const
{    return Name();   }
// *************************************
//
EventType           ParamKeyEvent::GetEventType         () const
{ return this->m_sEventType; }


//******************* ParamKeyEvent *************

// *************************************
//
void                ParamDescriptorEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( ParamCommand ) );

    ParamAddress.Serialize( ser );

    ser.EnterChild( SerializationHelper::REQUEST_STRING );
    ser.ExitChild();
}

// *************************************
//
IEventPtr           ParamDescriptorEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        ParamDescriptorEventPtr newEvent   = std::make_shared<ParamDescriptorEvent>();

        newEvent->ParamAddress      = ParameterAddress::Create( deser );
        newEvent->ParamCommand      = Convert::String2T<ParamDescriptorEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), ParamDescriptorEvent::Command::Fail );        
        newEvent->Request           = deser.DetachBranch( SerializationHelper::REQUEST_STRING );

        return newEvent;
    }
    return nullptr;    
}

// *************************************
//
IEventPtr               ParamDescriptorEvent::Clone             () const
{ return IEventPtr( new ParamDescriptorEvent( *this ) ); }



// *************************************
//
EventType               ParamDescriptorEvent::Type              ()
{    return m_sEventType;   }
// *************************************
//
std::string&            ParamDescriptorEvent::Name                ()
{    return m_sEventName;   }
// *************************************
//
const std::string &     ParamDescriptorEvent::GetName           () const
{    return Name();   }
// *************************************
//
EventType               ParamDescriptorEvent::GetEventType         () const
{ return this->m_sEventType; }



//******************* AssetEvent *************

// *************************************
//
void                    AssetEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( AssetCommand ) );
}

// *************************************
//
IEventPtr                AssetEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        AssetEventPtr newEvent        = std::make_shared< AssetEvent >();
        newEvent->AssetCommand      = Convert::String2T< AssetEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), AssetEvent::Command::Fail );
        
        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               AssetEvent::Clone             () const
{   return IEventPtr( new AssetEvent( *this ) );  }

// *************************************
//
EventType           AssetEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        AssetEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  AssetEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           AssetEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* SceneEvent *************

// *************************************
//
void                    SceneEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::NEW_SCENE_NAME_STRING, NewSceneName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( SceneCommand ) );
    ser.SetAttribute( SerializationHelper::ATTACH_INDEX_STRING, Convert::T2String( AttachIndex ) );
}

// *************************************
//
IEventPtr                SceneEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        SceneEventPtr newEvent        = std::make_shared< SceneEvent >();
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->NewSceneName      = deser.GetAttribute( SerializationHelper::NEW_SCENE_NAME_STRING );
        newEvent->SceneCommand      = Convert::String2T< SceneEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), SceneEvent::Command::Fail );
        newEvent->AttachIndex       = Convert::String2T<unsigned int>( deser.GetAttribute( SerializationHelper::ATTACH_INDEX_STRING ), 0 );
        
        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               SceneEvent::Clone             () const
{   return IEventPtr( new SceneEvent( *this ) );  }

// *************************************
//
EventType           SceneEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        SceneEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  SceneEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           SceneEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* LightEvent *************

// *************************************
//
void                    LightEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( SceneCommand ) );
    ser.SetAttribute( SerializationHelper::LIGHT_TYPE_STRING, LightType );
    ser.SetAttribute( SerializationHelper::LIGHT_INDEX_STRING, Convert::T2String( LightIndex ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_STRING, TimelinePath );
}

// *************************************
//
IEventPtr                LightEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        LightEventPtr newEvent        = std::make_shared< LightEvent >();
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->SceneCommand      = Convert::String2T< LightEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), LightEvent::Command::Fail );
        newEvent->LightType         = deser.GetAttribute( SerializationHelper::LIGHT_TYPE_STRING );
        newEvent->LightIndex        = Convert::String2T< UInt32 >( deser.GetAttribute( SerializationHelper::LIGHT_INDEX_STRING ), 0 );
        newEvent->TimelinePath      = deser.GetAttribute( SerializationHelper::TIMELINE_NAME_STRING );
        
        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               LightEvent::Clone             () const
{   return IEventPtr( new LightEvent( *this ) );  }

// *************************************
//
EventType           LightEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        LightEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  LightEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           LightEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* CameraEvent *************

// *************************************
//
void                    CameraEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( CameraCommand ) );
    ser.SetAttribute( SerializationHelper::LIGHT_INDEX_STRING, Convert::T2String( CameraIndex ) );
    //ser.SetAttribute( SerializationHelper::TIMELINE_NAME_STRING, TimelinePath );
}

// *************************************
//
IEventPtr                CameraEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        CameraEventPtr newEvent        = std::make_shared< CameraEvent >();
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->CameraCommand      = Convert::String2T< CameraEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), CameraEvent::Command::Fail );
        newEvent->CameraIndex        = Convert::String2T< UInt32 >( deser.GetAttribute( SerializationHelper::LIGHT_INDEX_STRING ), 0 );
        //newEvent->TimelinePath      = deser.GetAttribute( SerializationHelper::TIMELINE_NAME_STRING );
        
        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               CameraEvent::Clone             () const
{   return IEventPtr( new CameraEvent( *this ) );  }

// *************************************
//
EventType           CameraEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        CameraEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  CameraEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           CameraEvent::GetEventType() const
{   return this->m_sEventType; }




//******************* NodeStructureEvent *************

// *************************************
//
void                NodeStructureEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodePath );
    ser.SetAttribute( SerializationHelper::NEW_NODE_NAME_STRING, NewNodeName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( SceneCommand ) );
    ser.SetAttribute( SerializationHelper::ATTACH_INDEX_STRING, Convert::T2String( AttachIndex ) );

    ser.EnterChild( SerializationHelper::REQUEST_STRING );
    ser.ExitChild();
}

// *************************************
//
IEventPtr                NodeStructureEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        NodeStructureEventPtr newEvent   = std::make_shared<NodeStructureEvent>();
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->NodePath          = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
        newEvent->NewNodeName       = deser.GetAttribute( SerializationHelper::NEW_NODE_NAME_STRING );
        newEvent->SceneCommand      = Convert::String2T<NodeStructureEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), NodeStructureEvent::Command::Fail );
        newEvent->AttachIndex       = Convert::String2T<unsigned int>( deser.GetAttribute( SerializationHelper::ATTACH_INDEX_STRING ), 0 );

        newEvent->Request           = deser.DetachBranch( SerializationHelper::REQUEST_STRING );
        
        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               NodeStructureEvent::Clone             () const
{   return IEventPtr( new NodeStructureEvent( *this ) );  }

// *************************************
//
EventType           NodeStructureEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NodeStructureEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NodeStructureEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NodeStructureEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* PluginStructureEvent *************

// *************************************
//
void                PluginStructureEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodePath );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( PluginCommand ) );
    ser.SetAttribute( SerializationHelper::PLUGIN_NAME_STRING, PluginName );
    ser.SetAttribute( SerializationHelper::ATTACH_INDEX_STRING, Convert::T2String( AttachIndex ) );
    ser.SetAttribute( SerializationHelper::PLUGIN_UID_STRING, PluginUID );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_STRING, TimelinePath );

    ser.EnterChild( SerializationHelper::REQUEST_STRING );
    ser.ExitChild();
}

// *************************************
//
IEventPtr                PluginStructureEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        PluginStructureEventPtr newEvent   = std::make_shared<PluginStructureEvent>();
        newEvent->NodePath          = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->PluginName        = deser.GetAttribute( SerializationHelper::PLUGIN_NAME_STRING );
        newEvent->PluginCommand     = Convert::String2T<PluginStructureEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), PluginStructureEvent::Command::Fail );
        newEvent->AttachIndex       = Convert::String2T<unsigned int>( deser.GetAttribute( SerializationHelper::ATTACH_INDEX_STRING ), 0 );
        newEvent->PluginUID         = deser.GetAttribute( SerializationHelper::PLUGIN_UID_STRING );
        newEvent->TimelinePath      = deser.GetAttribute( SerializationHelper::TIMELINE_NAME_STRING );

        newEvent->Request            = deser.DetachBranch( SerializationHelper::REQUEST_STRING );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               PluginStructureEvent::Clone             () const
{   return IEventPtr( new PluginStructureEvent( *this ) );  }

// *************************************
//
EventType           PluginStructureEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        PluginStructureEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  PluginStructureEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           PluginStructureEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* ProjectEvent *************

// *************************************
//
void                ProjectEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( ProjectCommand ) );

    ser.EnterChild( SerializationHelper::REQUEST_STRING );
    ser.ExitChild();
}

// *************************************
//
IEventPtr                ProjectEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        ProjectEventPtr newEvent            = std::make_shared<ProjectEvent>();
        newEvent->ProjectCommand            = Convert::String2T<ProjectEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), ProjectEvent::Command::Fail );
        
        newEvent->Request                   = deser.DetachBranch( SerializationHelper::REQUEST_STRING );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               ProjectEvent::Clone             () const
{   return IEventPtr( new ProjectEvent( *this ) );  }

// *************************************
//
EventType           ProjectEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        ProjectEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  ProjectEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           ProjectEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* ResponseEvent *************

// *************************************
//
void                ResponseEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::RESPONSE_STRING, Response );
}

// *************************************
//
IEventPtr                ResponseEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        ResponseEventPtr newEvent   = std::make_shared<ResponseEvent>();
        newEvent->Response          = deser.GetAttribute( SerializationHelper::RESPONSE_STRING );
        
        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               ResponseEvent::Clone             () const
{   return IEventPtr( new ResponseEvent( *this ) );  }

// *************************************
//
EventType           ResponseEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        ResponseEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  ResponseEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           ResponseEvent::GetEventType() const
{   return this->m_sEventType; }



//******************* InfoEvent *************

// *************************************
//
void                InfoEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( InfoCommand ) );

    ser.EnterChild( SerializationHelper::REQUEST_STRING );
    ser.ExitChild();
}

// *************************************
//
IEventPtr                InfoEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        InfoEventPtr newEvent   = std::make_shared<InfoEvent>();
        newEvent->InfoCommand   = Convert::String2T<InfoEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), InfoEvent::Command::Fail );
        newEvent->Request       = deser.DetachBranch( SerializationHelper::REQUEST_STRING );
        
        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               InfoEvent::Clone             () const
{   return IEventPtr( new InfoEvent( *this ) );  }

// *************************************
//
EventType           InfoEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        InfoEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  InfoEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           InfoEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* TimeLineEvent *************

// *************************************
//
void                TimeLineEvent::Serialize            ( ISerializer & ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( TimelineCommand ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_STRING, TimelineName );
    ser.SetAttribute( SerializationHelper::TIMELINE_NEW_NAME_STRING, NewTimelineName );
    ser.SetAttribute( SerializationHelper::TIMELINE_TYPE_STRING, Convert::T2String( TimelineType ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_STRING, Convert::T2String( Time ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_DURATION_VALUE_STRING, Convert::T2String( Duration ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_WRAP_METHOD_STRING, Convert::T2String( WrapMethod ) );
}

// *************************************
//
IEventPtr                TimeLineEvent::Create          ( IDeserializer & deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        TimeLineEventPtr newEvent   = std::make_shared< TimeLineEvent >();

        newEvent->Time = Convert::String2T< TimeType >( deser.GetAttribute( SerializationHelper::KEY_TIME_STRING ), std::numeric_limits< TimeType >::quiet_NaN() );

        newEvent->TimelineCommand   = Convert::String2T< TimeLineEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), TimeLineEvent::Command::Fail );
        newEvent->TimelineName      = deser.GetAttribute( SerializationHelper::TIMELINE_NAME_STRING );
        newEvent->TimelineType      = Convert::String2T< bv::TimelineType >( deser.GetAttribute( SerializationHelper::TIMELINE_TYPE_STRING ), bv::TimelineType::TT_OFFSET );
        newEvent->Duration          = Convert::String2T< float >( deser.GetAttribute( SerializationHelper::TIMELINE_DURATION_VALUE_STRING ), 0.0f );
        newEvent->WrapMethod        = Convert::String2T< TimelineWrapMethod >( deser.GetAttribute( SerializationHelper::TIMELINE_WRAP_METHOD_STRING ), TimelineWrapMethod::TWM_CLAMP );
        newEvent->NewTimelineName   = deser.GetAttribute( SerializationHelper::TIMELINE_NEW_NAME_STRING );

        return newEvent;
    }
    return nullptr;    
}

// *************************************
//
IEventPtr           TimeLineEvent::Clone        () const
{   
    return IEventPtr( new TimeLineEvent( *this ) );  
}

// *************************************
//
EventType           TimeLineEvent::Type         ()
{   
    return m_sEventType;   
}
// *************************************
//
std::string &       TimeLineEvent::Name         ()
{   
    return m_sEventName;   
}

// *************************************
//
const std::string & TimeLineEvent::GetName      () const
{   
    return Name();   
}

// *************************************
//
EventType           TimeLineEvent::GetEventType () const
{   
    return this->m_sEventType;
}


//******************* TimerEvent *************

// *************************************
//
void                TimerEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( TimerCommand ) );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodeName );
    ser.SetAttribute( SerializationHelper::TIMER_HOURS_STRING, Convert::T2String( Hours ) );
    ser.SetAttribute( SerializationHelper::TIMER_MINUTES_STRING, Convert::T2String( Minutes ) );
    ser.SetAttribute( SerializationHelper::TIMER_SECONDS_STRING, Convert::T2String( Seconds ) );
    ser.SetAttribute( SerializationHelper::TIMER_TIME_PATERN_STRING, TimePatern );
    ser.SetAttribute( SerializationHelper::TIMER_MILLISECONDS_STRING, Convert::T2String( Milliseconds ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
}

// *************************************
//
IEventPtr                TimerEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        TimerEventPtr newEvent      = std::make_shared<TimerEvent>();
        newEvent->TimerCommand      = Convert::String2T<TimerEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), TimerEvent::Command::Fail );
        newEvent->NodeName          = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->Hours             = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_HOURS_STRING ), 0.0f );
        newEvent->Minutes           = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MINUTES_STRING ), 0.0f );
        newEvent->Seconds           = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_SECONDS_STRING ), 0.0f );
        newEvent->Milliseconds      = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MILLISECONDS_STRING ), 0.0f );
        newEvent->TimePatern        = deser.GetAttribute( SerializationHelper::TIMER_TIME_PATERN_STRING );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               TimerEvent::Clone             () const
{   return IEventPtr( new TimerEvent( *this ) );  }

// *************************************
//
EventType           TimerEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        TimerEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  TimerEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           TimerEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* VideoDecoderEvent *************

// *************************************
//
void                VideoDecoderEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( EventCommand ) );
    ser.SetAttribute( SerializationHelper::ASSET_PATH_STRING, AssetPath );
}

// *************************************
//
IEventPtr                VideoDecoderEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        VideoDecoderEventPtr newEvent   = std::make_shared< VideoDecoderEvent >();
        newEvent->EventCommand          = Convert::String2T< VideoDecoderEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), VideoDecoderEvent::Command::Fail );
        newEvent->AssetPath             = deser.GetAttribute( SerializationHelper::ASSET_PATH_STRING );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               VideoDecoderEvent::Clone             () const
{   return IEventPtr( new VideoDecoderEvent( *this ) );  }

// *************************************
//
EventType           VideoDecoderEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        VideoDecoderEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  VideoDecoderEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           VideoDecoderEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* NodeLogicEvent *************

// *************************************
//
void                NodeLogicEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( WidgetCommand ) );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodeName );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    
    ser.EnterChild( SerializationHelper::WIDGET_ACTION_STRING );
    ser.ExitChild();
}

// *************************************
//
IEventPtr                NodeLogicEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        NodeLogicEventPtr newEvent     = std::make_shared<NodeLogicEvent>();
        newEvent->WidgetCommand     = Convert::String2T<NodeLogicEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), NodeLogicEvent::Command::Fail );
        newEvent->NodeName          = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->Action            = deser.DetachBranch( SerializationHelper::WIDGET_ACTION_STRING );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               NodeLogicEvent::Clone             () const
{   return IEventPtr( new NodeLogicEvent( *this ) );  }

// *************************************
//
EventType           NodeLogicEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NodeLogicEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NodeLogicEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NodeLogicEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* VideoCardEvent *************

// *************************************
//
void                VideoCardEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( VideoCommand ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_NUMBER_STRING, Convert::T2String( Number ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_ID_STRING, Convert::T2String( VideoCardID ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_CHANNEL_ID_STRING, Convert::T2String( ChannelID ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_VALUE_STRING, Convert::T2String( Value ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_REFERENCE_MODE_STRING, Convert::T2String( Mode ) );
    
}

// *************************************
//
IEventPtr                VideoCardEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        VideoCardEventPtr newEvent      = std::make_shared< VideoCardEvent >();
        newEvent->VideoCommand          = Convert::String2T< VideoCardEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), VideoCardEvent::Command::Fail );
        newEvent->VideoCardID           = Convert::String2T< UInt32 >( deser.GetAttribute( SerializationHelper::VIDEO_CARD_ID_STRING ), 0 );
        newEvent->ChannelID             = Convert::String2T< UInt32 >( deser.GetAttribute( SerializationHelper::VIDEO_CARD_CHANNEL_ID_STRING ), 0 );
        newEvent->Mode                  = Convert::String2T< videocards::ReferenceMode >( deser.GetAttribute( SerializationHelper::VIDEO_CARD_REFERENCE_MODE_STRING ), videocards::ReferenceMode::FailMode );
        newEvent->Value                 = Convert::String2T< float >( deser.GetAttribute( SerializationHelper::VIDEO_CARD_VALUE_STRING ), 0.0f );
        newEvent->Number                = Convert::String2T< int >( deser.GetAttribute( SerializationHelper::VIDEO_CARD_NUMBER_STRING ), 0 );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               VideoCardEvent::Clone             () const
{   return IEventPtr( new VideoCardEvent( *this ) );  }

// *************************************
//
EventType           VideoCardEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        VideoCardEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  VideoCardEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           VideoCardEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* EngineStateEvent *************

// *************************************
//
void                EngineStateEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::REQUESTED_FPS_STRING, Convert::T2String( FPS ) );
    ser.SetAttribute( SerializationHelper::RENDERING_FILE_PATH, FilePath );
	ser.SetAttribute( SerializationHelper::GAIN_VAL_STRING, Convert::T2String( Gain ) );
    ser.SetAttribute( SerializationHelper::NUM_FRAMES_STRING, Convert::T2String( NumFrames ) );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( RenderingCommand ) );
}

// *************************************
//
IEventPtr                EngineStateEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        EngineStateEventPtr newEvent  = std::make_shared<EngineStateEvent>();
        newEvent->FilePath              = deser.GetAttribute( SerializationHelper::RENDERING_FILE_PATH );
        newEvent->FPS                   = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::REQUESTED_FPS_STRING ), 60 );
        newEvent->NumFrames             = Convert::String2T<int>( deser.GetAttribute( SerializationHelper::NUM_FRAMES_STRING ), 0 );
		newEvent->Gain					= Convert::String2T<float>( deser.GetAttribute( SerializationHelper::GAIN_VAL_STRING ), 1.f );
        newEvent->RenderingCommand      = Convert::String2T<EngineStateEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), EngineStateEvent::Command::Fail );
        newEvent->Request               = deser.DetachBranch( SerializationHelper::REQUEST_STRING );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               EngineStateEvent::Clone             () const
{   return IEventPtr( new EngineStateEvent( *this ) );  }

// *************************************
//
EventType           EngineStateEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        EngineStateEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  EngineStateEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           EngineStateEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* GlobalEffectEvent *************

// *************************************
//
void                GlobalEffectEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodePath );
    ser.SetAttribute( SerializationHelper::GLOBAL_EFFECT_NAME_STRING, EffectName );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_STRING, TimelinePath );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( EffectCommand ) );
}

// *************************************
//
IEventPtr                GlobalEffectEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        GlobalEffectEventPtr newEvent   = std::make_shared<GlobalEffectEvent>();
        newEvent->NodePath              = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
        newEvent->SceneName             = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->EffectName            = deser.GetAttribute( SerializationHelper::GLOBAL_EFFECT_NAME_STRING );
        newEvent->TimelinePath          = deser.GetAttribute( SerializationHelper::TIMELINE_NAME_STRING );
        newEvent->EffectCommand         = Convert::String2T<GlobalEffectEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), GlobalEffectEvent::Command::Fail );
        newEvent->AssetData             = deser.DetachBranch( SerializationHelper::ASSET_DATA_STRING );
        newEvent->Request               = deser.DetachBranch( SerializationHelper::REQUEST_STRING );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               GlobalEffectEvent::Clone             () const
{   return IEventPtr( new GlobalEffectEvent( *this ) );  }

// *************************************
//
EventType           GlobalEffectEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        GlobalEffectEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  GlobalEffectEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           GlobalEffectEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* TimelineKeyframeEvent *************

// *************************************
//
void                TimelineKeyframeEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_STRING, TimelinePath );
    ser.SetAttribute( SerializationHelper::KEYFRAME_NAME_STRING, KeyframeName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( KeyframeCommand ) );
    ser.SetAttribute( SerializationHelper::NEW_KEYFRAME_TYPE_STRING, Convert::T2String( NewKeyframeType ) );
    ser.SetAttribute( SerializationHelper::KEYFRAME_INDEX_STRING, Convert::T2String( KeyframeIndex ) );

    ser.SetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_STRING, Convert::T2String( Time ) );
    ser.SetAttribute( SerializationHelper::JUMP_TO_TIME_STRING, Convert::T2String( JumpToTime ) );
    ser.SetAttribute( SerializationHelper::TOTAL_LOOP_COUNT_STRING, Convert::T2String( TotalLoopCount ) );
    ser.SetAttribute( SerializationHelper::TRIGGER_EVENTS, TriggerEvents );
}

// *************************************
//
IEventPtr                TimelineKeyframeEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        TimelineKeyframeEventPtr newEvent   = std::make_shared<TimelineKeyframeEvent>();
        newEvent->TimelinePath          = deser.GetAttribute( SerializationHelper::TIMELINE_NAME_STRING );
        newEvent->KeyframeName          = deser.GetAttribute( SerializationHelper::KEYFRAME_NAME_STRING );
        newEvent->KeyframeCommand       = Convert::String2T<TimelineKeyframeEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), TimelineKeyframeEvent::Command::Fail );
        newEvent->NewKeyframeType       = Convert::String2T<TimelineKeyframeEvent::KeyframeType>( deser.GetAttribute( SerializationHelper::NEW_KEYFRAME_TYPE_STRING ), TimelineKeyframeEvent::KeyframeType::KeyframeTypeFail );
        newEvent->KeyframeIndex         = Convert::String2T( deser.GetAttribute( SerializationHelper::KEYFRAME_INDEX_STRING ), -1 );

        newEvent->Time                  = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_STRING ), std::numeric_limits<float>::quiet_NaN() );
        newEvent->JumpToTime            = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::JUMP_TO_TIME_STRING ), std::numeric_limits<float>::quiet_NaN() );
        newEvent->TotalLoopCount        = Convert::String2T<unsigned int>( deser.GetAttribute( SerializationHelper::TOTAL_LOOP_COUNT_STRING ), 0 );
        newEvent->TriggerEvents         = deser.GetAttribute( SerializationHelper::TRIGGER_EVENTS );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               TimelineKeyframeEvent::Clone             () const
{   return IEventPtr( new TimelineKeyframeEvent( *this ) );  }

// *************************************
//
EventType           TimelineKeyframeEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        TimelineKeyframeEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  TimelineKeyframeEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           TimelineKeyframeEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* MouseEvent *************

// *************************************
//
void                MouseEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );

    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( MouseCommand ) );
    ser.SetAttribute( SerializationHelper::MOUSE_X_STRING, Convert::T2String( MouseX ) );
    ser.SetAttribute( SerializationHelper::MOUSE_Y_STRING, Convert::T2String( MouseY ) );
    ser.SetAttribute( SerializationHelper::MOUSE_AUTO_SELECT_FLAG, Convert::T2String( AutoSelect ) );
}

// *************************************
//
IEventPtr                MouseEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        MouseEventPtr newEvent      = std::make_shared<MouseEvent>();
        newEvent->MouseX            = Convert::String2T<Float32>( deser.GetAttribute( SerializationHelper::MOUSE_X_STRING ), -1.0f );
        newEvent->MouseY            = Convert::String2T<Float32>( deser.GetAttribute( SerializationHelper::MOUSE_Y_STRING ), -1.0f );
        newEvent->AutoSelect        = Convert::String2T<bool>( deser.GetAttribute( SerializationHelper::MOUSE_AUTO_SELECT_FLAG ), true );
        newEvent->AutoSelectColor   = Convert::String2T<glm::vec4>( deser.GetAttribute( SerializationHelper::MOUSE_AUTO_SELECT_COLOR ), glm::vec4( 1, 1, 1, 1 ) );
        newEvent->MouseCommand      = Convert::String2T( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), MouseEvent::Command::Fail );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               MouseEvent::Clone             () const
{   return IEventPtr( new MouseEvent( *this ) );  }

// *************************************
//
EventType           MouseEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        MouseEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  MouseEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           MouseEvent::GetEventType() const
{   return this->m_sEventType; }

//******************* SceneVariableEvent *************

// *************************************
//
void                SceneVariableEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );

    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( VariableCommand ) );
    ser.SetAttribute( SerializationHelper::SCENE_VARIABLE_NAME_STRING, Convert::T2String( VariableName ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::SCENE_VARAIBLE_CONTENT_STRING, VariableContent );
}

// *************************************
//
IEventPtr           SceneVariableEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        SceneVariableEventPtr newEvent      = std::make_shared<SceneVariableEvent>();
        newEvent->SceneName                 = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->VariableName              = deser.GetAttribute( SerializationHelper::SCENE_VARIABLE_NAME_STRING );
        newEvent->VariableContent           = deser.GetAttribute( SerializationHelper::SCENE_VARAIBLE_CONTENT_STRING );
        newEvent->VariableCommand           = Convert::String2T( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), SceneVariableEvent::Command::Fail );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr           SceneVariableEvent::Clone             () const
{   return IEventPtr( new SceneVariableEvent( *this ) );  }

// *************************************
//
EventType           SceneVariableEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        SceneVariableEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  SceneVariableEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           SceneVariableEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* ConfigEvent *************

// *************************************
//
void                ConfigEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );

    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( ConfigCommand ) );
    ser.SetAttribute( SerializationHelper::CONFIG_VALUE_STRING, Convert::T2String( Value ) );
    ser.SetAttribute( SerializationHelper::CONFIG_KEY_STRING, Key );
}

// *************************************
//
IEventPtr           ConfigEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        ConfigEventPtr newEvent             = std::make_shared<ConfigEvent>();
        newEvent->Value                     = deser.GetAttribute( SerializationHelper::CONFIG_VALUE_STRING );
        newEvent->Key                       = deser.GetAttribute( SerializationHelper::CONFIG_KEY_STRING );
        newEvent->ConfigCommand             = Convert::String2T( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), ConfigEvent::Command::Fail );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr           ConfigEvent::Clone             () const
{   return IEventPtr( new ConfigEvent( *this ) );  }

// *************************************
//
EventType           ConfigEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        ConfigEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  ConfigEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           ConfigEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* GridLineEvent *************

// *************************************
//
void                GridLineEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );

    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( GridLineCommand ) );
    ser.SetAttribute( SerializationHelper::GRIDLINE_NAME_STRING, GridLineName );
    ser.SetAttribute( SerializationHelper::GRIDLINE_TYPE_STRING, GridLineType );
    ser.SetAttribute( SerializationHelper::GRIDLINE_POSITION_STRING, Convert::T2String< float >( GridLinePosition ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodeName );
    ser.SetAttribute( SerializationHelper::GRIDLINE_INDEX_STRING, Convert::T2String( GridLineIndex ) );
    ser.SetAttribute( SerializationHelper::GRIDLINE_ALIGNEMENT_TYPE_STRING, AlignementType );
}

// *************************************
//
IEventPtr           GridLineEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        GridLineEventPtr newEvent               = std::make_shared<GridLineEvent>();
        newEvent->SceneName                     = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->NodeName                      = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
        newEvent->GridLineName                  = deser.GetAttribute( SerializationHelper::GRIDLINE_NAME_STRING );
        newEvent->GridLineType                  = deser.GetAttribute( SerializationHelper::GRIDLINE_TYPE_STRING );
        newEvent->GridLinePosition              = Convert::String2T( deser.GetAttribute( SerializationHelper::GRIDLINE_POSITION_STRING ), 0.0f );
        newEvent->GridLineIndex                 = Convert::String2T( deser.GetAttribute( SerializationHelper::GRIDLINE_INDEX_STRING ), 0 );
        newEvent->AlignementType                = deser.GetAttribute( SerializationHelper::GRIDLINE_ALIGNEMENT_TYPE_STRING );

        newEvent->GridLineCommand               = Convert::String2T( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), GridLineEvent::Command::Fail );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr           GridLineEvent::Clone             () const
{   return IEventPtr( new GridLineEvent( *this ) );  }

// *************************************
//
EventType           GridLineEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        GridLineEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  GridLineEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           GridLineEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* UndoRedoEvent *************

// *************************************
//
void                UndoRedoEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );

    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( UndoCommand ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::UNDO_REDO_NUM_STEPS_STRING, Convert::T2String( (int)NumSteps ) );
    ser.SetAttribute( SerializationHelper::UNDO_REDO_OPERATION_BUFF_SIZE_STRING, Convert::T2String( (int)Size ) );
}

// *************************************
//
IEventPtr           UndoRedoEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        UndoRedoEventPtr newEvent               = std::make_shared<UndoRedoEvent>();
        newEvent->SceneName                     = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->NumSteps                      = (UInt16)Convert::String2T( deser.GetAttribute( SerializationHelper::UNDO_REDO_NUM_STEPS_STRING ), 0 );
        newEvent->Size                          = (UInt16)Convert::String2T( deser.GetAttribute( SerializationHelper::UNDO_REDO_OPERATION_BUFF_SIZE_STRING ), 0 );
        newEvent->UndoCommand                   = Convert::String2T( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), UndoRedoEvent::Command::Fail );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr           UndoRedoEvent::Clone             () const
{   return IEventPtr( new UndoRedoEvent( *this ) );  }

// *************************************
//
EventType           UndoRedoEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        UndoRedoEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  UndoRedoEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           UndoRedoEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* GenericEvent *************

// *************************************
//
void                GenericEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );

    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( CommandName ) );
}

// *************************************
//
IEventPtr           GenericEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        GenericEventPtr newEvent = std::make_shared< GenericEvent >();
        newEvent->CommandName = deser.GetAttribute( SerializationHelper::COMMAND_STRING );
        newEvent->Request = deser.DetachBranch( SerializationHelper::REQUEST_STRING );

        return newEvent;
    }
    return nullptr;
}
// *************************************
//
IEventPtr           GenericEvent::Clone             () const
{
    return IEventPtr( new GenericEvent( *this ) );
}

// *************************************
//
EventType           GenericEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        GenericEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  GenericEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           GenericEvent::GetEventType() const
{
    return this->m_sEventType;
}




//******************* HightmapEvent *************

// *************************************
//
void                HightmapEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( HightmapCommand ) );
    ser.SetAttribute( SerializationHelper::TIMER_HOURS_STRING, Convert::T2String( Hours ) );
    ser.SetAttribute( SerializationHelper::TIMER_MINUTES_STRING, Convert::T2String( Minutes ) );
    ser.SetAttribute( SerializationHelper::TIMER_SECONDS_STRING, Convert::T2String( Seconds ) );
    ser.SetAttribute( SerializationHelper::TIMER_MILLISECONDS_STRING, Convert::T2String( Milliseconds ) );
}

// *************************************
//
IEventPtr                HightmapEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        HightmapEventPtr newEvent   = std::make_shared<HightmapEvent>();
        newEvent->HightmapCommand   = Convert::String2T<HightmapEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), HightmapEvent::Command::Fail );
        newEvent->Hours             = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_HOURS_STRING ), 0.0f );
        newEvent->Minutes           = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MINUTES_STRING ), 0.0f );
        newEvent->Seconds           = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_SECONDS_STRING ), 0.0f );
        newEvent->Milliseconds      = Convert::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MILLISECONDS_STRING ), 0.0f );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               HightmapEvent::Clone             () const
{   return IEventPtr( new HightmapEvent( *this ) );  }

// *************************************
//
EventType           HightmapEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        HightmapEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  HightmapEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           HightmapEvent::GetEventType() const
{   return this->m_sEventType; }


// ************************************* UnqueueAudioInternalEvent *************************************

// *************************************
//
uintptr_t AssetTrackerInternalEvent::m_sDefaultUID = std::numeric_limits< uintptr_t >::max();


// *************************************
//
AssetTrackerInternalEvent::AssetTrackerInternalEvent        ( Command eventCommand )
    : EventCommand( eventCommand )
    , AssetUID( m_sDefaultUID )
    , AssetKey( std::string() )
{
}

// *************************************
//
AssetTrackerInternalEvent::AssetTrackerInternalEvent        ( Command eventCommand, uintptr_t assetUID )
    : EventCommand( eventCommand )
    , AssetUID( assetUID )
    , AssetKey( std::string() )
{
}

// *************************************
//
AssetTrackerInternalEvent::AssetTrackerInternalEvent        ( Command eventCommand, std::string assetKey )
    : EventCommand( eventCommand )
    , AssetUID( m_sDefaultUID )
    , AssetKey( assetKey )
{
}

// *************************************
//
void                AssetTrackerInternalEvent::Serialize              ( ISerializer& ser ) const
{    assert( !"Should not be serialized" );  }

// *************************************
//
IEventPtr           AssetTrackerInternalEvent::Create                 ( IDeserializer& deser )
{
    assert( !"Should not be deserialized" );
    return nullptr;    
}

// *************************************
//
IEventPtr           AssetTrackerInternalEvent::Clone                  () const
{   
    return IEventPtr( new AssetTrackerInternalEvent( *this ) );  
}

// *************************************
//
EventType           AssetTrackerInternalEvent::Type                   ()
{   
    return m_sEventType;   
}

// *************************************
//
std::string &       AssetTrackerInternalEvent::Name                   ()
{   
    return m_sEventName;   
}

// *************************************
//
const std::string & AssetTrackerInternalEvent::GetName                () const
{   
    return Name();   
}

// *************************************
//
EventType           AssetTrackerInternalEvent::GetEventType           () const
{   
    return this->m_sEventType; 
}

// *************************************
//
bool                AssetTrackerInternalEvent::HasUID                   () const
{
    return ( AssetUID != AssetTrackerInternalEvent::m_sDefaultUID );
}

// *************************************
//
bool                AssetTrackerInternalEvent::HasKey                   () const
{
    return ( !AssetKey.empty() );
}



//******************* GizmoEvent *************

// *************************************
//
void                GizmoEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, Convert::T2String( CommandName ) );
    Address.Serialize( ser );
}

// *************************************
//
IEventPtr           GizmoEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        GizmoEventPtr newEvent = std::make_shared< GizmoEvent >();
        newEvent->CommandName = Convert::String2T( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), GizmoEvent::Command::Fail );
        newEvent->Address = GizmoAddress::Create( deser );

        return newEvent;
    }
    return nullptr;
}
// *************************************
//
IEventPtr           GizmoEvent::Clone             () const
{
    return IEventPtr( new GizmoEvent( *this ) );
}

// *************************************
//
EventType           GizmoEvent::Type()
{
    return m_sEventType;
}
// *************************************
//
std::string&        GizmoEvent::Name()
{
    return m_sEventName;
}
// *************************************
//
const std::string&  GizmoEvent::GetName() const
{
    return Name();
}
// *************************************
//
EventType           GizmoEvent::GetEventType() const
{
    return this->m_sEventType;
}



#pragma warning( pop )

} //bv
