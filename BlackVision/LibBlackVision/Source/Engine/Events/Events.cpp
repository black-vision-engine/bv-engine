#include "stdafx.h"

#include "Events.h"

#include <cassert>

#pragma warning( push )
#pragma warning(disable:4100)

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"


#include <limits>

namespace bv 
{

// ************************************* Static Events' Definitions *************************************

const EventType PluginAddedEvent::m_sEventType      = 0x00000001;
std::string PluginAddedEvent::m_sEventName          = "Event_PluginAdded";

const EventType NodeAddedEvent::m_sEventType        = 0x00000002;
std::string NodeAddedEvent::m_sEventName            = "Event_NodeAdded";

const EventType FrameRenderedEvent::m_sEventType    = 0x00000003;
std::string FrameRenderedEvent::m_sEventName        = "Event_FrameRendered";

const EventType TransformSetEvent::m_sEventType     = 0x00000004;
std::string TransformSetEvent::m_sEventName         = "Event_TransformSet";

const EventType KeyPressedEvent::m_sEventType       = 0x00000005;
std::string KeyPressedEvent::m_sEventName           = "Event_KeyPressedEvent";

const EventType ScreenShotRenderedEvent::m_sEventType       = 0x00000006;
std::string ScreenShotRenderedEvent::m_sEventName           = "ScreenShotRenderedEvent";

// FIXME: MORE EVENTS IN ScrollerEvents.cpp. VERY VERY BAD IDEA. WE NEED EVENTS REGISTER SYSTEM ASAP.


const EventType LoadAssetEvent::m_sEventType        = 0x30000013;
std::string LoadAssetEvent::m_sEventName            = "LoadAssetEvent";

const EventType ParamKeyEvent::m_sEventType         = 0x30000006;
std::string ParamKeyEvent::m_sEventName             = "ParamKeyEvent";

const EventType NodeStructureEvent::m_sEventType    = 0x30000012;
std::string NodeStructureEvent::m_sEventName        = "NodeStructureEvent";

const EventType PluginStructureEvent::m_sEventType  = 0x30000015;
std::string PluginStructureEvent::m_sEventName      = "PluginStructureEvent";

const EventType ProjectEvent::m_sEventType          = 0x30000001;
std::string ProjectEvent::m_sEventName              = "ProjectEvent";

const EventType ResponseEvent::m_sEventType         = 0x30000008;
std::string ResponseEvent::m_sEventName             = "ResponseEvent";

const EventType InfoEvent::m_sEventType             = 0x30000007;
std::string InfoEvent::m_sEventName                 = "InfoEvent";

const EventType TimeLineEvent::m_sEventType         = 0x30000009;
std::string TimeLineEvent::m_sEventName             = "TimeLineEvent";

const EventType TimerEvent::m_sEventType            = 0x30000010;
std::string TimerEvent::m_sEventName                = "TimerEvent";

const EventType NodeLogicEvent::m_sEventType           = 0x30000011;
std::string NodeLogicEvent::m_sEventName               = "NodeLogicEvent";

const EventType VideoCardEvent::m_sEventType        = 0x30000005;
std::string VideoCardEvent::m_sEventName            = "VideoCardEvent";

const EventType HightmapEvent::m_sEventType         = 0x30000014;
std::string HightmapEvent::m_sEventName             = "HightmapEvent";

const EventType EngineStateEvent::m_sEventType       = 0x30000016;
std::string EngineStateEvent::m_sEventName           = "EngineStateEvent";

const EventType SceneEvent::m_sEventType			= 0x30000017;
std::string SceneEvent::m_sEventName				= "SceneEvent";

const EventType AssetEvent::m_sEventType			= 0x30000018;
std::string AssetEvent::m_sEventName				= "AssetEvent";


const EventType GlobalEffectEvent::m_sEventType     = 0x30000019;
std::string GlobalEffectEvent::m_sEventName         = "GlobalEffectEvent";

const EventType TimelineKeyframeEvent::m_sEventType = 0x30000020;
std::string TimelineKeyframeEvent::m_sEventName     = "TimelineKeyframeEvent";

const EventType MouseEvent::m_sEventType			= 0x30000021;
std::string MouseEvent::m_sEventName				= "MouseEvent";

// ************************************* Events Serialization *****************************************

namespace SerializationHelper
{
// Key names used to serialization.

const std::string EVENT_TYPE_STRING       = "Event";
const std::string NODE_NAME_STRING        = "NodeName";
const std::string PLUGIN_NAME_STRING      = "PluginName";
const std::string TIMELINE_NAME_STRING    = "TimelineName";      // TimeLineEvent and NodeStructureEvent
const std::string COMMAND_STRING          = "Command";
const std::string REQUEST_STRING          = "Request";           // ProjectStructureEvent InfoEvent

const std::string RESPONSE_COMMAND_STRING   = "cmd";

static const char* EMPTY_STRING = "";

// ========================================================================= //
// LoadAssetEvent
// ========================================================================= //
const std::string ASSET_DATA_STRING       = "AssetData";

std::pair< LoadAssetEvent::Command, const char* > LoadAssetMapping[] = 
    { std::make_pair( LoadAssetEvent::Command::LoadAsset, "LoadAsset" )
    , std::make_pair( LoadAssetEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> LoadAssetEvent::Command  String2T        ( const std::string& s, const LoadAssetEvent::Command& defaultVal )     { return String2Enum( LoadAssetMapping, s, defaultVal ); }
template<> std::string              T2String        ( const LoadAssetEvent::Command & t )                                   { return Enum2String( LoadAssetMapping, t ); }

// ========================================================================= //
// ParamKeyEvent
// ========================================================================= //
const std::string PARAM_NAME_STRING           = "ParamName";
const std::string PARAM_SUB_NAME_STRING       = "ParamSubName";
const std::string PARAM_VALUE_STRING          = "ParamValue";
const std::string KEY_TIME_STRING             = "Time";
const std::string PARAM_TARGET_TYPE_STRING    = "Target";

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
    , std::make_pair( ParamKeyEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> ParamKeyEvent::Command   String2T    ( const std::string& s, const ParamKeyEvent::Command& defaultVal )      { return String2Enum( ParameterCommandMapping, s, defaultVal ); }
template<> std::string              T2String    ( const ParamKeyEvent::Command & t )                                    { return Enum2String( ParameterCommandMapping, t ); }


std::pair< ParamKeyEvent::TargetType, const char* > TargetTypeMapping[] = 
    { std::make_pair( ParamKeyEvent::TargetType::GlobalEffectParam, "GlobalEffectParam" )
    , std::make_pair( ParamKeyEvent::TargetType::PluginParam, "PluginParam" ) 
    , std::make_pair( ParamKeyEvent::TargetType::ResourceParam, "ResourceParam" ) 
    , std::make_pair( ParamKeyEvent::TargetType::FailTarget, SerializationHelper::EMPTY_STRING )      // default
};

template<> ParamKeyEvent::TargetType    String2T            ( const std::string& s, const ParamKeyEvent::TargetType& defaultVal )       { return String2Enum( TargetTypeMapping, s, defaultVal ); }
template<> std::string                  T2String            ( const ParamKeyEvent::TargetType & t )                                     { return Enum2String( TargetTypeMapping, t ); }

// ========================================================================= //
// AssetEvent
// ========================================================================= //

std::pair< AssetEvent::Command, const char* > AssetCommandMapping[] = 
{
    std::make_pair( AssetEvent::Command::ClearUnusedCachedAssets, "ClearUnusedCachedAssets" )
    , std::make_pair( AssetEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> AssetEvent::Command  String2T    ( const std::string& s, const AssetEvent::Command& defaultVal )     { return String2Enum( AssetCommandMapping, s, defaultVal ); }
template<> std::string          T2String    ( const AssetEvent::Command & t )                                   { return Enum2String( AssetCommandMapping, t ); }


// ========================================================================= //
// SceneEvent
// ========================================================================= //
const std::string NEW_SCENE_NAME_STRING        = "NewSceneName";

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
    , std::make_pair( SceneEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> SceneEvent::Command  String2T    ( const std::string& s, const SceneEvent::Command& defaultVal )     { return String2Enum( SceneCommandMapping, s, defaultVal ); }
template<> std::string          T2String    ( const SceneEvent::Command & t )                                   { return Enum2String( SceneCommandMapping, t ); }


// ========================================================================= //
// NodeStructureEvent
// ========================================================================= //
const std::string NEW_NODE_NAME_STRING        = "NewNodeName";

std::pair< NodeStructureEvent::Command, const char* > NodeStructureCommandMapping[] = 
{
    std::make_pair( NodeStructureEvent::Command::AddNode, "AddNode" )
    , std::make_pair( NodeStructureEvent::Command::RemoveNode, "RemoveNode" ) 
    , std::make_pair( NodeStructureEvent::Command::SetNodeInvisible, "SetNodeInvisible" ) 
    , std::make_pair( NodeStructureEvent::Command::SetNodeVisible, "SetNodeVisible" )
	, std::make_pair( NodeStructureEvent::Command::RenameNode, "RenameNode" )
	, std::make_pair( NodeStructureEvent::Command::AttachNode, "AttachNode" )
	, std::make_pair( NodeStructureEvent::Command::DetachNode, "DetachNode" )
	, std::make_pair( NodeStructureEvent::Command::MoveNode, "MoveNode" )
	, std::make_pair( NodeStructureEvent::Command::CopyNode, "CopyNode" )
    , std::make_pair( NodeStructureEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> NodeStructureEvent::Command  String2T   ( const std::string& s, const NodeStructureEvent::Command& defaultVal )     { return String2Enum( NodeStructureCommandMapping, s, defaultVal ); }
template<> std::string                  T2String   ( const NodeStructureEvent::Command & t )                                   { return Enum2String( NodeStructureCommandMapping, t ); }

// ========================================================================= //
// PluginStructureEvent
// ========================================================================= //
const std::string ATTACH_INDEX_STRING				= "AttachIndex";
const std::string PLUGIN_UID_STRING               = "PluginUID";

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

template<> PluginStructureEvent::Command    String2T   ( const std::string& s, const PluginStructureEvent::Command& defaultVal )   { return String2Enum( PluginStructureCommandMapping, s, defaultVal ); }
template<> std::string                      T2String   ( const PluginStructureEvent::Command & t )                                 { return Enum2String( PluginStructureCommandMapping, t ); }


// ========================================================================= //
// ProjectEvent
// ========================================================================= //
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

    , std::make_pair( ProjectEvent::Command::CopyAsset, "CopyAsset" ) 
    , std::make_pair( ProjectEvent::Command::MoveAsset, "MoveAsset" ) 
    , std::make_pair( ProjectEvent::Command::RemoveAsset, "RemoveAsset" ) 
    , std::make_pair( ProjectEvent::Command::ImportAsset, "ImportAsset" ) 
    , std::make_pair( ProjectEvent::Command::SavePreset, "SavePreset" ) 
    , std::make_pair( ProjectEvent::Command::LoadPreset, "LoadPreset" ) 
    , std::make_pair( ProjectEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> ProjectEvent::Command    String2T   ( const std::string& s, const ProjectEvent::Command& defaultVal )   { return String2Enum( ProjectEventCommandMapping, s, defaultVal ); }
template<> std::string              T2String   ( const ProjectEvent::Command & t )                                 { return Enum2String( ProjectEventCommandMapping, t ); }

// ========================================================================= //
// ResponseEvent
// ========================================================================= //
const std::string RESPONSE_STRING                     = "Response";

// ========================================================================= //
// InfoEvent
// ========================================================================= //

std::pair< InfoEvent::Command, const char* > InfoEventCommandMapping[] = 
{
    std::make_pair( InfoEvent::Command::TreeStructure, "TreeStructure" )
    , std::make_pair( InfoEvent::Command::MinimalTreeStructure, "MinimalTreeStructure" )
    , std::make_pair( InfoEvent::Command::NodeInfo, "NodeInfo" )
    , std::make_pair( InfoEvent::Command::PluginInfo, "PluginInfo" )
    , std::make_pair( InfoEvent::Command::MinimalSceneInfo, "MinimalSceneInfo" )

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

    , std::make_pair( InfoEvent::Command::Timelines, "TimeLines" ) 
    , std::make_pair( InfoEvent::Command::ListTimelineKeyframes, "ListTimelineKeyframes" ) 
    , std::make_pair( InfoEvent::Command::CheckTimelineTime, "CheckTimelineTime" )
    , std::make_pair( InfoEvent::Command::ListTimelinesParams, "ListTimelinesParams" )

    , std::make_pair( InfoEvent::Command::Performance, "Performance" )
    , std::make_pair( InfoEvent::Command::Videocards, "VideoCards" )
    , std::make_pair( InfoEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> InfoEvent::Command   String2T     ( const std::string& s, const InfoEvent::Command& defaultVal )     { return String2Enum( InfoEventCommandMapping, s, defaultVal ); }
template<> std::string          T2String     ( const InfoEvent::Command & t )                                   { return Enum2String( InfoEventCommandMapping, t ); }

// ========================================================================= //
// TimeLineEvent
// ========================================================================= //
const std::string TIMELINE_TYPE_STRING                = "Type";
const std::string TIMELINE_TIME_VALUE_STRING          = "Time";
const std::string TIMELINE_DURATION_VALUE_STRING      = "Duration";
const std::string TIMELINE_NEW_NAME_STRING            = "TimelineNewName";
const std::string TIMELINE_WRAP_METHOD_STRING			= "WrapMethod";
const std::string SCENE_NAME_STRING                   = "SceneName";

// Timeline Command

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

template<> TimeLineEvent::Command   String2T    ( const std::string& s, const TimeLineEvent::Command& defaultVal )         { return String2Enum( TimeLineEventCommandMapping, s, defaultVal ); }
template<> std::string              T2String    ( const TimeLineEvent::Command & t )                                       { return Enum2String( TimeLineEventCommandMapping, t ); }


// Timeline Type

std::pair< TimelineType, const char* > TimelineTypeMapping[] = 
{
    std::make_pair( TimelineType::TT_DEFAULT, "Default" )
    , std::make_pair( TimelineType::TT_OFFSET, "Offset" ) 
    , std::make_pair( TimelineType::TT_CONST, "Const" ) 
    , std::make_pair( TimelineType::TT_TOTAL, SerializationHelper::EMPTY_STRING )      // default
};

template<> TimelineType         String2T    ( const std::string& s, const TimelineType& defaultVal )   { return String2Enum( TimelineTypeMapping, s, defaultVal ); }
template<> std::string          T2String    ( const TimelineType & t )                                 { return Enum2String( TimelineTypeMapping, t ); }


// ========================================================================= //
// NodeLogicEvent
// ========================================================================= //
const std::string WIDGET_ACTION_STRING                = "Action";
const std::string WIDGET_TIME_VALUE_STRING            = "Time";

std::pair< NodeLogicEvent::Command, const char* > NodeLogicEventCommandMapping[] = 
{
    std::make_pair( NodeLogicEvent::Command::AddNodeLogic, "AddNodeLogic" )
    , std::make_pair( NodeLogicEvent::Command::DeleteNodeLogic, "DeleteNodeLogic" )
    , std::make_pair( NodeLogicEvent::Command::LogicAction, "LogicAction" ) 
    , std::make_pair( NodeLogicEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> NodeLogicEvent::Command  String2T        ( const std::string& s, const NodeLogicEvent::Command& defaultVal )     { return String2Enum( NodeLogicEventCommandMapping, s, defaultVal ); }
template<> std::string              T2String        ( const NodeLogicEvent::Command & t )                                   { return Enum2String( NodeLogicEventCommandMapping, t ); }

// ========================================================================= //
// VideoCardEvent
// ========================================================================= //
const std::string VIDEO_CARD_NUMBER_STRING            = "Number";
const std::string VIDEO_CARD_VALUE_STRING             = "Value";
const std::string VIDEO_CARD_REFERENCE_MODE_STRING    = "ReferenceMode";


std::pair< VideoCardEvent::Command, const char* > VideoCardEventCommandMapping[] = 
{
    std::make_pair( VideoCardEvent::Command::EnableOutput, "EnableOutput" )
    , std::make_pair( VideoCardEvent::Command::DisableOutput, "DisableOutput" ) 
    , std::make_pair( VideoCardEvent::Command::EnableKey, "EnableKey" ) 
    , std::make_pair( VideoCardEvent::Command::DisableKey, "DisableKey" )
    , std::make_pair( VideoCardEvent::Command::ReferenceMode, "ReferenceMode" )
    , std::make_pair( VideoCardEvent::Command::ReferenceOffsetH, "ReferenceOffsetH" ) 
    , std::make_pair( VideoCardEvent::Command::ReferenceOffsetV, "ReferenceOffsetV" ) 
    , std::make_pair( VideoCardEvent::Command::EnableInput, "EnableInput" )
    , std::make_pair( VideoCardEvent::Command::DisableInput, "DisableInput" )
    , std::make_pair( VideoCardEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> VideoCardEvent::Command  String2T    ( const std::string& s, const VideoCardEvent::Command& defaultVal )         { return String2Enum( VideoCardEventCommandMapping, s, defaultVal ); }
template<> std::string              T2String    ( const VideoCardEvent::Command & t )                                       { return Enum2String( VideoCardEventCommandMapping, t ); }


std::pair< VideoCardEvent::VideoReferenceMode, const char* > VideoCardVideoReferenceModeMapping[] = 
{
    std::make_pair( VideoCardEvent::VideoReferenceMode::FreeRun, "FreeRun" )
    , std::make_pair( VideoCardEvent::VideoReferenceMode::AnalogBlackBurst, "AnalogBlackBurst" ) 
    , std::make_pair( VideoCardEvent::VideoReferenceMode::AnalogTriLevel, "AnalogTriLevel" ) 
    , std::make_pair( VideoCardEvent::VideoReferenceMode::DigitalInput1, "DigitalInput1" )
    , std::make_pair( VideoCardEvent::VideoReferenceMode::DigitalInput2, "DigitalInput2" )
    , std::make_pair( VideoCardEvent::VideoReferenceMode::FailMode, SerializationHelper::EMPTY_STRING )      // default
};

template<> VideoCardEvent::VideoReferenceMode   String2T   ( const std::string& s, const VideoCardEvent::VideoReferenceMode& defaultVal )  { return String2Enum( VideoCardVideoReferenceModeMapping, s, defaultVal ); }
template<> std::string                          T2String   ( const VideoCardEvent::VideoReferenceMode & t )                                { return Enum2String( VideoCardVideoReferenceModeMapping, t ); }


// ========================================================================= //
// TimerEvent
// ========================================================================= //
const std::string TIMER_HOURS_STRING                  = "Hours";
const std::string TIMER_MINUTES_STRING                = "Minutes";
const std::string TIMER_SECONDS_STRING                = "Seconds";
const std::string TIMER_MILLISECONDS_STRING           = "Milliseconds";

std::pair< TimerEvent::Command, const char* > TimerEventCommandMapping[] = 
{
    std::make_pair( TimerEvent::Command::Start, "Start" )
    , std::make_pair( TimerEvent::Command::Stop, "Stop" ) 
    , std::make_pair( TimerEvent::Command::Reset, "Reset" ) 
    , std::make_pair( TimerEvent::Command::SetTime, "SetTime" )
    , std::make_pair( TimerEvent::Command::SetTimeStart, "SetTimeStart" )
    , std::make_pair( TimerEvent::Command::SetTimeStop, "SetTimeStop" ) 
    , std::make_pair( TimerEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> TimerEvent::Command  String2T    ( const std::string& s, const TimerEvent::Command& defaultVal )     { return String2Enum( TimerEventCommandMapping, s, defaultVal ); }
template<> std::string          T2String    ( const TimerEvent::Command & t )                                   { return Enum2String( TimerEventCommandMapping, t ); }

// ========================================================================= //
// EngineStateEvent
// ========================================================================= //
const std::string REQUESTED_FPS_STRING                = "FPS";
const std::string NUM_FRAMES_STRING                   = "NumberFrames";
const std::string RENDERING_FILE_PATH                  = "FilePath";

std::pair< EngineStateEvent::Command, const char* > EngineStateEventCommandMapping[] = 
{
    std::make_pair( EngineStateEvent::Command::ScreenShot, "ScreenShot" )
    , std::make_pair( EngineStateEvent::Command::RenderOffscreen, "RenderOffscreen" )
    , std::make_pair( EngineStateEvent::Command::CloseApplication, "CloseApplication" )
    , std::make_pair( EngineStateEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> EngineStateEvent::Command String2T     ( const std::string& s, const EngineStateEvent::Command& defaultVal )     { return String2Enum( EngineStateEventCommandMapping, s, defaultVal ); }
template<> std::string T2String                   ( const EngineStateEvent::Command & t )                                   { return Enum2String( EngineStateEventCommandMapping, t ); }

// ========================================================================= //
// GlobalEffectEvent
// ========================================================================= //
const std::string GLOBAL_EFFECT_NAME_STRING                = "GlobalEffectName";

std::pair< GlobalEffectEvent::Command, const char* > GlobalEffectEventCommandMapping[] = 
{
    std::make_pair( GlobalEffectEvent::Command::SetGlobalEffect, "SetGlobalEffect" )
    , std::make_pair( GlobalEffectEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> GlobalEffectEvent::Command   String2T    ( const std::string& s, const GlobalEffectEvent::Command& defaultVal )  { return String2Enum( GlobalEffectEventCommandMapping, s, defaultVal ); }
template<> std::string                  T2String    ( const GlobalEffectEvent::Command & t )                                { return Enum2String( GlobalEffectEventCommandMapping, t ); }


// ========================================================================= //
// TimelineKeyframeEvent
// ========================================================================= //
const std::string KEYFRAME_NAME_STRING                = "KeyframeName";
const std::string KEYFRAME_INDEX_STRING               = "KeyframeIndex";
const std::string NEW_KEYFRAME_TYPE_STRING            = "NewKeyframeType";
const std::string JUMP_TO_TIME_STRING                 = "JumpToTime";
const std::string TOTAL_LOOP_COUNT_STRING             = "TotalLoopCount";

std::pair< TimelineKeyframeEvent::Command, const char* > TimelineKeyframeEventCommandMapping[] = 
{
    std::make_pair( TimelineKeyframeEvent::Command::AddKeyframe, "AddKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::Command::RemoveKeyframe, "RemoveKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> TimelineKeyframeEvent::Command   String2T   ( const std::string& s, const TimelineKeyframeEvent::Command& defaultVal )      { return String2Enum( TimelineKeyframeEventCommandMapping, s, defaultVal ); }
template<> std::string                      T2String   ( const TimelineKeyframeEvent::Command & t )                                    { return Enum2String( TimelineKeyframeEventCommandMapping, t ); }

std::pair< TimelineKeyframeEvent::KeyframeType, const char* > KeyframeTypeMapping[] = 
{
    std::make_pair( TimelineKeyframeEvent::KeyframeType::StopKeyframe, "StopKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::LoopReverseKeyframe, "LoopReverseKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::LoopJumpKeyframe, "LoopJumpKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::LoopRestartKeyframe, "LoopRestartKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::NullKeyframe, "NullKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::KeyframeTypeFail, SerializationHelper::EMPTY_STRING )      // default
};

template<> TimelineKeyframeEvent::KeyframeType  String2T   ( const std::string& s, const TimelineKeyframeEvent::KeyframeType& defaultVal )      { return String2Enum( KeyframeTypeMapping, s, defaultVal ); }
template<> std::string                          T2String   ( const TimelineKeyframeEvent::KeyframeType & t )                                    { return Enum2String( KeyframeTypeMapping, t ); }

// ========================================================================= //
// MouseEvent
// ========================================================================= //

const std::string MOUSE_X_STRING                = "MouseX";
const std::string MOUSE_Y_STRING                = "MouseY";

std::pair< MouseEvent::Command, const char* > MouseEventCommandMapping[] = 
{
    std::make_pair( MouseEvent::Command::MouseUp, "MouseUp" )
    , std::make_pair( MouseEvent::Command::MouseDown, "MouseDown" )
    , std::make_pair( MouseEvent::Command::MouseMove, "MouseMove" )
    , std::make_pair( MouseEvent::Command::Fail, SerializationHelper::EMPTY_STRING )      // default
};

template<> MouseEvent::Command      String2T   ( const std::string & s, const MouseEvent::Command & defaultVal )    { return String2Enum( MouseEventCommandMapping, s, defaultVal ); }
template<> std::string              T2String   ( const MouseEvent::Command & t )                                    { return Enum2String( MouseEventCommandMapping, t ); }


// ========================================================================= //
// HightmapEvent
// ========================================================================= //
const std::string COMMAND_HM_ENABLE_STRING    = "Enable";
const std::string COMMAND_HM_START_STRING     = "Start";
const std::string COMMAND_HM_RESET_STRING     = "Reset";
const std::string COMMAND_HM_SHOW_STRING      = "Show";
const std::string COMMAND_HM_ZOOM_STRING      = "Zoom";
const std::string COMMAND_HM_ZOOM_OUT_STRING  = "ZoomOut";
const std::string COMMAND_HM_SET_STRING       = "Set";
const std::string COMMAND_HM_ANIM_STRING      = "Anim";
const std::string COMMAND_HM_ANIM2_STRING     = "Anim2";
const std::string COMMAND_HM_SET3_STRING      = "Set3";

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


template<> HightmapEvent::Command   String2T    ( const std::string& s, const HightmapEvent::Command& defaultVal )      { return String2Enum( HightmapEventCommandMapping, s, defaultVal ); }
template<> std::string              T2String    ( const HightmapEvent::Command & t )                                    { return Enum2String( HightmapEventCommandMapping, t ); }

}

// ************************************* PluginAddedEvent *************************************

// *************************************
//
PluginAddedEvent::PluginAddedEvent                          ()
    : m_addedPlugin( nullptr )
{}

// *************************************
//
PluginAddedEvent::PluginAddedEvent                          ( model::IPluginConstPtr plugin )
    : m_addedPlugin( plugin )
{}

// *************************************
//
void                PluginAddedEvent::Serialize            ( ISerializer& ser ) const
{    assert( !"Should not be serialized" );  }

// *************************************
//
IEventPtr                PluginAddedEvent::Create          ( IDeserializer& deser )
{
    assert( !"Should not be deserialized" );
    return nullptr;    
}
// *************************************
//
IEventPtr               PluginAddedEvent::Clone             () const
{   return IEventPtr( new PluginAddedEvent( *this ) );  }

// *************************************
//
EventType           PluginAddedEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        PluginAddedEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  PluginAddedEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           PluginAddedEvent::GetEventType() const
{   return this->m_sEventType; }

// *************************************
//
model::IPluginConstPtr  PluginAddedEvent::GetPlugin         () const
{    return m_addedPlugin;  }

// ************************************* NodeAddedEvent *************************************

// *************************************
//
NodeAddedEvent::NodeAddedEvent                              ()
    : m_addedNode( nullptr )
{}
// *************************************
//
NodeAddedEvent::NodeAddedEvent                              ( model::IModelNodeConstPtr node )
    : m_addedNode( node )
{}

// *************************************
//
void                NodeAddedEvent::Serialize            ( ISerializer& ser ) const
{    assert( !"Should not be serialized" );  }

// *************************************
//
IEventPtr                NodeAddedEvent::Create          ( IDeserializer& deser )
{
    assert( !"Should not be deserialized" );
    return nullptr;    
}
// *************************************
//
IEventPtr               NodeAddedEvent::Clone             () const
{   return IEventPtr( new NodeAddedEvent( *this ) );  }

// *************************************
//
EventType           NodeAddedEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NodeAddedEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NodeAddedEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NodeAddedEvent::GetEventType() const
{   return this->m_sEventType; }

// *************************************
//
model::IModelNodeConstPtr  NodeAddedEvent::GetNode         () const
{
    return m_addedNode;
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
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodeName );
    ser.SetAttribute( SerializationHelper::PLUGIN_NAME_STRING, PluginName );
    ser.SetAttribute( SerializationHelper::PARAM_NAME_STRING, ParamName );
    ser.SetAttribute( SerializationHelper::PARAM_SUB_NAME_STRING, ParamSubName );
    ser.SetAttribute( SerializationHelper::PARAM_VALUE_STRING, Value );
    ser.SetAttribute( SerializationHelper::KEY_TIME_STRING, SerializationHelper::T2String( Time ) );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( ParamCommand ) );
    ser.SetAttribute( SerializationHelper::PARAM_TARGET_TYPE_STRING, SerializationHelper::T2String( ParamTargetType ) );
}

// *************************************
//
IEventPtr           ParamKeyEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        ParamKeyEventPtr newEvent   = std::make_shared<ParamKeyEvent>();
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING);
        newEvent->PluginName        = deser.GetAttribute( SerializationHelper::PLUGIN_NAME_STRING );
        newEvent->NodeName          = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
        newEvent->ParamName         = deser.GetAttribute( SerializationHelper::PARAM_NAME_STRING );
        newEvent->ParamSubName      = deser.GetAttribute( SerializationHelper::PARAM_SUB_NAME_STRING );
        newEvent->Value             = deser.GetAttribute( SerializationHelper::PARAM_VALUE_STRING );
        newEvent->ParamCommand      = SerializationHelper::String2T<ParamKeyEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), ParamKeyEvent::Command::Fail );
        newEvent->ParamTargetType   = SerializationHelper::String2T<ParamKeyEvent::TargetType>( deser.GetAttribute( SerializationHelper::PARAM_TARGET_TYPE_STRING ), ParamKeyEvent::TargetType::FailTarget );
        newEvent->Time              = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::KEY_TIME_STRING ), std::numeric_limits<float>::quiet_NaN() );

        return newEvent;
    }
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


//******************* AssetEvent *************

// *************************************
//
void					AssetEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( AssetCommand ) );
}

// *************************************
//
IEventPtr                AssetEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        AssetEventPtr newEvent		= std::make_shared< AssetEvent >();
        newEvent->AssetCommand      = SerializationHelper::String2T< AssetEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), AssetEvent::Command::Fail );
        
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
void					SceneEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::NEW_SCENE_NAME_STRING, NewSceneName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( SceneCommand ) );
    ser.SetAttribute( SerializationHelper::ATTACH_INDEX_STRING, SerializationHelper::T2String( AttachIndex ) );
}

// *************************************
//
IEventPtr                SceneEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        SceneEventPtr newEvent		= std::make_shared< SceneEvent >();
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->NewSceneName      = deser.GetAttribute( SerializationHelper::NEW_SCENE_NAME_STRING );
        newEvent->SceneCommand      = SerializationHelper::String2T< SceneEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), SceneEvent::Command::Fail );
        newEvent->AttachIndex       = SerializationHelper::String2T<unsigned int>( deser.GetAttribute( SerializationHelper::ATTACH_INDEX_STRING ), 0 );
        
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


//******************* NodeStructureEvent *************

// *************************************
//
void                NodeStructureEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodePath );
    ser.SetAttribute( SerializationHelper::NEW_NODE_NAME_STRING, NewNodeName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( SceneCommand ) );
    ser.SetAttribute( SerializationHelper::ATTACH_INDEX_STRING, SerializationHelper::T2String( AttachIndex ) );

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
        newEvent->SceneCommand      = SerializationHelper::String2T<NodeStructureEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), NodeStructureEvent::Command::Fail );
        newEvent->AttachIndex       = SerializationHelper::String2T<unsigned int>( deser.GetAttribute( SerializationHelper::ATTACH_INDEX_STRING ), 0 );

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
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( PluginCommand ) );
    ser.SetAttribute( SerializationHelper::PLUGIN_NAME_STRING, PluginName );
    ser.SetAttribute( SerializationHelper::ATTACH_INDEX_STRING, SerializationHelper::T2String( AttachIndex ) );
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
        newEvent->PluginCommand     = SerializationHelper::String2T<PluginStructureEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), PluginStructureEvent::Command::Fail );
        newEvent->AttachIndex       = SerializationHelper::String2T<unsigned int>( deser.GetAttribute( SerializationHelper::ATTACH_INDEX_STRING ), 0 );
        newEvent->PluginUID         = deser.GetAttribute( SerializationHelper::PLUGIN_UID_STRING );
        newEvent->TimelinePath      = deser.GetAttribute( SerializationHelper::TIMELINE_NAME_STRING );

        newEvent->Request			= deser.DetachBranch( SerializationHelper::REQUEST_STRING );

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
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( ProjectCommand ) );

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
        newEvent->ProjectCommand            = SerializationHelper::String2T<ProjectEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), ProjectEvent::Command::Fail );
        
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
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( InfoCommand ) );

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
        newEvent->InfoCommand   = SerializationHelper::String2T<InfoEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), InfoEvent::Command::Fail );
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
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( TimelineCommand ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_STRING, TimelineName );
    ser.SetAttribute( SerializationHelper::TIMELINE_NEW_NAME_STRING, NewTimelineName );
    ser.SetAttribute( SerializationHelper::TIMELINE_TYPE_STRING, SerializationHelper::T2String( TimelineType ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_STRING, SerializationHelper::T2String( Time ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_DURATION_VALUE_STRING, SerializationHelper::T2String( Duration ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_WRAP_METHOD_STRING, SerializationHelper::T2String( WrapMethod ) );
}

// *************************************
//
IEventPtr                TimeLineEvent::Create          ( IDeserializer & deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        TimeLineEventPtr newEvent   = std::make_shared< TimeLineEvent >();
        newEvent->Time              = SerializationHelper::String2T< float >( deser.GetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_STRING ), std::numeric_limits<float>::quiet_NaN() );
        newEvent->TimelineCommand   = SerializationHelper::String2T< TimeLineEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), TimeLineEvent::Command::Fail );
        newEvent->TimelineName      = deser.GetAttribute( SerializationHelper::TIMELINE_NAME_STRING );
        newEvent->TimelineType      = SerializationHelper::String2T< bv::TimelineType >( deser.GetAttribute( SerializationHelper::TIMELINE_TYPE_STRING ), bv::TimelineType::TT_OFFSET );
        newEvent->Duration          = SerializationHelper::String2T< float >( deser.GetAttribute( SerializationHelper::TIMELINE_DURATION_VALUE_STRING ), 0.0f );
        newEvent->WrapMethod        = SerializationHelper::String2T< TimelineWrapMethod >( deser.GetAttribute( SerializationHelper::TIMELINE_WRAP_METHOD_STRING ), TimelineWrapMethod::TWM_CLAMP );
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
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( TimerCommand ) );
    ser.SetAttribute( SerializationHelper::NODE_NAME_STRING, NodeName );
    ser.SetAttribute( SerializationHelper::TIMER_HOURS_STRING, SerializationHelper::T2String( Hours ) );
    ser.SetAttribute( SerializationHelper::TIMER_MINUTES_STRING, SerializationHelper::T2String( Minutes ) );
    ser.SetAttribute( SerializationHelper::TIMER_SECONDS_STRING, SerializationHelper::T2String( Seconds ) );
    ser.SetAttribute( SerializationHelper::TIMER_MILLISECONDS_STRING, SerializationHelper::T2String( Milliseconds ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_STRING, SceneName );
}

// *************************************
//
IEventPtr                TimerEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        TimerEventPtr newEvent      = std::make_shared<TimerEvent>();
        newEvent->TimerCommand      = SerializationHelper::String2T<TimerEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), TimerEvent::Command::Fail );
        newEvent->NodeName          = deser.GetAttribute( SerializationHelper::NODE_NAME_STRING );
        newEvent->SceneName         = deser.GetAttribute( SerializationHelper::SCENE_NAME_STRING );
        newEvent->Hours             = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_HOURS_STRING ), 0.0f );
        newEvent->Minutes           = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MINUTES_STRING ), 0.0f );
        newEvent->Seconds           = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_SECONDS_STRING ), 0.0f );
        newEvent->Milliseconds      = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MILLISECONDS_STRING ), 0.0f );

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


//******************* NodeLogicEvent *************

// *************************************
//
void                NodeLogicEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( WidgetCommand ) );
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
        newEvent->WidgetCommand     = SerializationHelper::String2T<NodeLogicEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), NodeLogicEvent::Command::Fail );
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
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( VideoCommand ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_NUMBER_STRING, SerializationHelper::T2String( Number ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_VALUE_STRING, SerializationHelper::T2String( Value ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_REFERENCE_MODE_STRING, SerializationHelper::T2String( Mode ) );
}

// *************************************
//
IEventPtr                VideoCardEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        VideoCardEventPtr newEvent      = std::make_shared<VideoCardEvent>();
        newEvent->VideoCommand          = SerializationHelper::String2T<VideoCardEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), VideoCardEvent::Command::Fail );
        newEvent->Mode                  = SerializationHelper::String2T<VideoCardEvent::VideoReferenceMode>( deser.GetAttribute( SerializationHelper::VIDEO_CARD_REFERENCE_MODE_STRING ), VideoCardEvent::VideoReferenceMode::FailMode );
        newEvent->Value                 = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::VIDEO_CARD_VALUE_STRING ), 0.0f );
        newEvent->Number                = SerializationHelper::String2T<int>( deser.GetAttribute( SerializationHelper::VIDEO_CARD_NUMBER_STRING ), 0 );

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
    ser.SetAttribute( SerializationHelper::REQUESTED_FPS_STRING, SerializationHelper::T2String( FPS ) );
    ser.SetAttribute( SerializationHelper::RENDERING_FILE_PATH, FilePath );
    ser.SetAttribute( SerializationHelper::NUM_FRAMES_STRING, SerializationHelper::T2String( NumFrames ) );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( RenderingCommand ) );
}

// *************************************
//
IEventPtr                EngineStateEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        EngineStateEventPtr newEvent  = std::make_shared<EngineStateEvent>();
        newEvent->FilePath              = deser.GetAttribute( SerializationHelper::RENDERING_FILE_PATH );
        newEvent->FPS                   = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::REQUESTED_FPS_STRING ), 60 );
        newEvent->NumFrames             = SerializationHelper::String2T<int>( deser.GetAttribute( SerializationHelper::NUM_FRAMES_STRING ), 0 );
        newEvent->RenderingCommand      = SerializationHelper::String2T<EngineStateEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), EngineStateEvent::Command::Fail );

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
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( EffectCommand ) );
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
        newEvent->EffectCommand         = SerializationHelper::String2T<GlobalEffectEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), GlobalEffectEvent::Command::Fail );

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
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( KeyframeCommand ) );
    ser.SetAttribute( SerializationHelper::NEW_KEYFRAME_TYPE_STRING, SerializationHelper::T2String( NewKeyframeType ) );
    ser.SetAttribute( SerializationHelper::KEYFRAME_INDEX_STRING, SerializationHelper::T2String( KeyframeIndex ) );

    ser.SetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_STRING, SerializationHelper::T2String( Time ) );
    ser.SetAttribute( SerializationHelper::JUMP_TO_TIME_STRING, SerializationHelper::T2String( JumpToTime ) );
    ser.SetAttribute( SerializationHelper::TOTAL_LOOP_COUNT_STRING, SerializationHelper::T2String( TotalLoopCount ) );
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
        newEvent->KeyframeCommand       = SerializationHelper::String2T<TimelineKeyframeEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), TimelineKeyframeEvent::Command::Fail );
        newEvent->NewKeyframeType       = SerializationHelper::String2T<TimelineKeyframeEvent::KeyframeType>( deser.GetAttribute( SerializationHelper::NEW_KEYFRAME_TYPE_STRING ), TimelineKeyframeEvent::KeyframeType::KeyframeTypeFail );
        newEvent->KeyframeIndex         = SerializationHelper::String2T( deser.GetAttribute( SerializationHelper::KEYFRAME_INDEX_STRING ), -1 );

        newEvent->Time                  = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_STRING ), std::numeric_limits<float>::quiet_NaN() );
        newEvent->JumpToTime            = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::JUMP_TO_TIME_STRING ), std::numeric_limits<float>::quiet_NaN() );
        newEvent->TotalLoopCount        = SerializationHelper::String2T<unsigned int>( deser.GetAttribute( SerializationHelper::TOTAL_LOOP_COUNT_STRING ), 0 );

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

    ser.SetAttribute( SerializationHelper::MOUSE_X_STRING, SerializationHelper::T2String( MouseX ) );
    ser.SetAttribute( SerializationHelper::MOUSE_Y_STRING, SerializationHelper::T2String( MouseY ) );
}

// *************************************
//
IEventPtr                MouseEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        MouseEventPtr newEvent   = std::make_shared<MouseEvent>();
        newEvent->MouseX    = SerializationHelper::String2T<Int32>( deser.GetAttribute( SerializationHelper::MOUSE_Y_STRING ), -1 );
        newEvent->MouseY    = SerializationHelper::String2T<Int32>( deser.GetAttribute( SerializationHelper::MOUSE_Y_STRING ), -1 );

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



//******************* HightmapEvent *************

// *************************************
//
void                HightmapEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_STRING, m_sEventName );
    ser.SetAttribute( SerializationHelper::COMMAND_STRING, SerializationHelper::T2String( HightmapCommand ) );
    ser.SetAttribute( SerializationHelper::TIMER_HOURS_STRING, SerializationHelper::T2String( Hours ) );
    ser.SetAttribute( SerializationHelper::TIMER_MINUTES_STRING, SerializationHelper::T2String( Minutes ) );
    ser.SetAttribute( SerializationHelper::TIMER_SECONDS_STRING, SerializationHelper::T2String( Seconds ) );
    ser.SetAttribute( SerializationHelper::TIMER_MILLISECONDS_STRING, SerializationHelper::T2String( Milliseconds ) );
}

// *************************************
//
IEventPtr                HightmapEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_STRING ) == m_sEventName )
    {
        HightmapEventPtr newEvent   = std::make_shared<HightmapEvent>();
        newEvent->HightmapCommand   = SerializationHelper::String2T<HightmapEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_STRING ), HightmapEvent::Command::Fail );
        newEvent->Hours             = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_HOURS_STRING ), 0.0f );
        newEvent->Minutes           = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MINUTES_STRING ), 0.0f );
        newEvent->Seconds           = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_SECONDS_STRING ), 0.0f );
        newEvent->Milliseconds      = SerializationHelper::String2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MILLISECONDS_STRING ), 0.0f );

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


#pragma warning( pop )

} //bv
