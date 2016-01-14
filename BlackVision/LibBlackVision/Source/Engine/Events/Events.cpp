#include "Events.h"

#include <cassert>

#pragma warning( push )
#pragma warning(disable:4100)

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "EventHelpers.h"

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

// FIXME: MORE EVENTS IN CrawlerEvents.cpp. VERY VERY BAD IDEA. WE NEED EVENTS REGISTER SYSTEM ASAP.


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

const EventType WidgetEvent::m_sEventType           = 0x30000011;
std::string WidgetEvent::m_sEventName               = "WidgetEvent";

const EventType VideoCardEvent::m_sEventType        = 0x30000005;
std::string VideoCardEvent::m_sEventName            = "VideoCardEvent";

const EventType HightmapEvent::m_sEventType         = 0x30000014;
std::string HightmapEvent::m_sEventName             = "HightmapEvent";

const EventType RenderingModeEvent::m_sEventType       = 0x30000016;
std::string RenderingModeEvent::m_sEventName           = "RenderingModeEvent";

const EventType SceneEvent::m_sEventType			= 0x30000017;
std::string SceneEvent::m_sEventName				= "SceneEvent";

const EventType AssetEvent::m_sEventType			= 0x30000018;
std::string AssetEvent::m_sEventName				= "AssetEvent";


const EventType GlobalEffectEvent::m_sEventType			= 0x30000019;
std::string GlobalEffectEvent::m_sEventName				= "GlobalEffectEvent";

const EventType TimelineKeyframeEvent::m_sEventType			= 0x30000020;
std::string TimelineKeyframeEvent::m_sEventName				= "TimelineKeyframeEvent";


// ************************************* Events Serialization *****************************************

namespace SerializationHelper
{
// Key names used to serialization.

const std::wstring EVENT_TYPE_WSTRING       = L"Event";
const std::wstring NODE_NAME_WSTRING        = L"NodeName";
const std::wstring PLUGIN_NAME_WSTRING      = L"PluginName";
const std::wstring TIMELINE_NAME_WSTRING    = L"TimelineName";      // TimeLineEvent and NodeStructureEvent
const std::wstring COMMAND_WSTRING          = L"Command";
const std::wstring REQUEST_WSTRING          = L"Request";           // ProjectStructureEvent InfoEvent

const std::string RESPONSE_COMMAND_STRING   = "cmd";


// ========================================================================= //
// LoadAssetEvent
// ========================================================================= //
const std::wstring ASSET_DATA_WSTRING       = L"AssetData";

std::pair< LoadAssetEvent::Command, const std::wstring > LoadAssetMapping[] = 
    { std::make_pair( LoadAssetEvent::Command::LoadAsset, L"LoadAsset" )
    , std::make_pair( LoadAssetEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> LoadAssetEvent::Command WString2T    ( const std::wstring& s, const LoadAssetEvent::Command& defaultVal )    { return WString2T( LoadAssetMapping, s, defaultVal ); }
template<> const std::wstring& T2WString        ( LoadAssetEvent::Command t )                                           { return Enum2WString( LoadAssetMapping, t ); }

// ========================================================================= //
// ParamKeyEvent
// ========================================================================= //
const std::wstring PARAM_NAME_WSTRING           = L"ParamName";
const std::wstring PARAM_VALUE_WSTRING          = L"ParamValue";
const std::wstring KEY_TIME_WSTRING             = L"Time";
const std::wstring PARAM_TARGET_TYPE_WSTRING    = L"Target";

std::pair< ParamKeyEvent::Command, const std::wstring > ParameterCommandMapping[] = 
    { std::make_pair( ParamKeyEvent::Command::AddKey, L"AddKey" )
    , std::make_pair( ParamKeyEvent::Command::RemoveKey, L"RemoveKey" ) 
    , std::make_pair( ParamKeyEvent::Command::UpdateKey, L"UpdateKey" ) 
    , std::make_pair( ParamKeyEvent::Command::SetInterpolatorType, L"SetInterpolator" ) 
    , std::make_pair( ParamKeyEvent::Command::SetInterpolatorPreWrapMethod, L"SetInterpolatorPreWrapMethod" )
    , std::make_pair( ParamKeyEvent::Command::SetInterpolatorPostWrapMethod, L"SetInterpolatorPostWrapMethod" )
    , std::make_pair( ParamKeyEvent::Command::AssignTimeline, L"AssignTimeline" )
    , std::make_pair( ParamKeyEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> ParamKeyEvent::Command WString2T ( const std::wstring& s, const ParamKeyEvent::Command& defaultVal )     { return WString2T( ParameterCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString    ( ParamKeyEvent::Command t )                                            { return Enum2WString( ParameterCommandMapping, t ); }


std::pair< ParamKeyEvent::TargetType, const std::wstring > TargetTypeMapping[] = 
    { std::make_pair( ParamKeyEvent::TargetType::GlobalEffectParam, L"GlobalEffectParam" )
    , std::make_pair( ParamKeyEvent::TargetType::PluginParam, L"PluginParam" ) 
    , std::make_pair( ParamKeyEvent::TargetType::ResourceParam, L"ResourceParam" ) 
    , std::make_pair( ParamKeyEvent::TargetType::FailTarget, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> ParamKeyEvent::TargetType WString2T      ( const std::wstring& s, const ParamKeyEvent::TargetType& defaultVal )      { return WString2T( TargetTypeMapping, s, defaultVal ); }
template<> const std::wstring& T2WString            ( ParamKeyEvent::TargetType t )                                             { return Enum2WString( TargetTypeMapping, t ); }

// ========================================================================= //
// AssetEvent
// ========================================================================= //

std::pair< AssetEvent::Command, const std::wstring > AssetCommandMapping[] = 
{
    std::make_pair( AssetEvent::Command::ClearUnusedCachedAssets, L"ClearUnusedCachedAssets" )
    , std::make_pair( AssetEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> AssetEvent::Command WString2T    ( const std::wstring& s, const AssetEvent::Command& defaultVal )    { return WString2T( AssetCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString    ( AssetEvent::Command t )                                           { return Enum2WString( AssetCommandMapping, t ); }


// ========================================================================= //
// SceneEvent
// ========================================================================= //
const std::wstring NEW_SCENE_NAME_WSTRING        = L"NewSceneName";

std::pair< SceneEvent::Command, const std::wstring > SceneCommandMapping[] = 
{
    std::make_pair( SceneEvent::Command::AddScene, L"AddScene" )
    , std::make_pair( SceneEvent::Command::RemoveScene, L"RemoveScene" )
    , std::make_pair( SceneEvent::Command::RemoveAllScenes, L"RemoveAllScenes" )
    , std::make_pair( SceneEvent::Command::SetSceneVisible, L"SetSceneVisible" ) 
    , std::make_pair( SceneEvent::Command::SetSceneInvisible, L"SetSceneInvisible" )
	, std::make_pair( SceneEvent::Command::RenameScene, L"RenameScene" )
	, std::make_pair( SceneEvent::Command::AttachScene, L"AttachScene" )
	, std::make_pair( SceneEvent::Command::DetachScene, L"DetachScene" )
	, std::make_pair( SceneEvent::Command::MoveScene, L"MoveScene" )
	, std::make_pair( SceneEvent::Command::CopyScene, L"CopyScene" )
    , std::make_pair( SceneEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> SceneEvent::Command WString2T    ( const std::wstring& s, const SceneEvent::Command& defaultVal )    { return WString2T( SceneCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString    ( SceneEvent::Command t )                                           { return Enum2WString( SceneCommandMapping, t ); }


// ========================================================================= //
// NodeStructureEvent
// ========================================================================= //
const std::wstring NEW_NODE_NAME_WSTRING        = L"NewNodeName";

std::pair< NodeStructureEvent::Command, const std::wstring > NodeStructureCommandMapping[] = 
{
    std::make_pair( NodeStructureEvent::Command::AddNode, L"AddNode" )
    , std::make_pair( NodeStructureEvent::Command::RemoveNode, L"RemoveNode" ) 
    , std::make_pair( NodeStructureEvent::Command::SetNodeInvisible, L"SetNodeInvisible" ) 
    , std::make_pair( NodeStructureEvent::Command::SetNodeVisible, L"SetNodeVisible" )
	, std::make_pair( NodeStructureEvent::Command::RenameNode, L"RenameNode" )
	, std::make_pair( NodeStructureEvent::Command::AttachNode, L"AttachNode" )
	, std::make_pair( NodeStructureEvent::Command::DetachNode, L"DetachNode" )
	, std::make_pair( NodeStructureEvent::Command::MoveNode, L"MoveNode" )
	, std::make_pair( NodeStructureEvent::Command::CopyNode, L"CopyNode" )
    , std::make_pair( NodeStructureEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> NodeStructureEvent::Command  WString2T   ( const std::wstring& s, const NodeStructureEvent::Command& defaultVal )    { return WString2T( NodeStructureCommandMapping, s, defaultVal ); }
template<> const std::wstring&          T2WString   ( NodeStructureEvent::Command t )                                           { return Enum2WString( NodeStructureCommandMapping, t ); }

// ========================================================================= //
// PluginStructureEvent
// ========================================================================= //
const std::wstring ATTACH_INDEX_WSTRING				= L"AttachIndex";
const std::wstring PLUGIN_UID_WSTRING               = L"PluginUID";

std::pair< PluginStructureEvent::Command, const std::wstring > PluginStructureCommandMapping[] = 
{
    std::make_pair( PluginStructureEvent::Command::AttachPlugin, L"AttachPlugin" )
    , std::make_pair( PluginStructureEvent::Command::DetachPlugin, L"DetachPlugin" ) 
    , std::make_pair( PluginStructureEvent::Command::AddPlugin, L"AddPlugin" ) 
    , std::make_pair( PluginStructureEvent::Command::RemovePlugin, L"RemovePlugin" )
    , std::make_pair( PluginStructureEvent::Command::CopyPlugin, L"CopyPlugin" )
    , std::make_pair( PluginStructureEvent::Command::MovePlugin, L"MovePlugin" )
    , std::make_pair( PluginStructureEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> PluginStructureEvent::Command    WString2T   ( const std::wstring& s, const PluginStructureEvent::Command& defaultVal )  { return WString2T( PluginStructureCommandMapping, s, defaultVal ); }
template<> const std::wstring&              T2WString   ( PluginStructureEvent::Command t )                                         { return Enum2WString( PluginStructureCommandMapping, t ); }


// ========================================================================= //
// ProjectEvent
// ========================================================================= //
std::pair< ProjectEvent::Command, const std::wstring > ProjectEventCommandMapping[] = 
{
    std::make_pair( ProjectEvent::Command::SaveScene, L"SaveScene" )
    , std::make_pair( ProjectEvent::Command::LoadScene, L"LoadScene" )
    , std::make_pair( ProjectEvent::Command::RemoveScene, L"RemoveScene" )
    , std::make_pair( ProjectEvent::Command::CopyScene, L"CopyScene" )
    , std::make_pair( ProjectEvent::Command::MoveScene, L"MoveScene" )

    , std::make_pair( ProjectEvent::Command::AddExistingSceneToProject, L"AddExistingSceneToProject" )
    , std::make_pair( ProjectEvent::Command::RemoveSceneFromProject, L"RemoveSceneFromProject" )
    , std::make_pair( ProjectEvent::Command::CreateSceneInProject, L"CreateSceneInProject" )

    , std::make_pair( ProjectEvent::Command::LoadProject, L"LoadProject" ) 
    , std::make_pair( ProjectEvent::Command::NewProject, L"NewProject" ) 
    , std::make_pair( ProjectEvent::Command::SetCurrentProject, L"SetCurrentProject" )
    , std::make_pair( ProjectEvent::Command::MoveProject, L"MoveProject" )
    , std::make_pair( ProjectEvent::Command::DeleteProject, L"DeleteProject" )
    , std::make_pair( ProjectEvent::Command::RenameProject, L"RenameProject" )

    , std::make_pair( ProjectEvent::Command::CreateFolder, L"CreateFolder" )
    , std::make_pair( ProjectEvent::Command::DeleteFolder, L"DeleteFolder" )

    , std::make_pair( ProjectEvent::Command::CopyAsset, L"CopyAsset" ) 
    , std::make_pair( ProjectEvent::Command::MoveAsset, L"MoveAsset" ) 
    , std::make_pair( ProjectEvent::Command::RemoveAsset, L"RemoveAsset" ) 
    , std::make_pair( ProjectEvent::Command::ImportAsset, L"ImportAsset" ) 
    , std::make_pair( ProjectEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> ProjectEvent::Command WString2T  ( const std::wstring& s, const ProjectEvent::Command& defaultVal )  { return WString2T( ProjectEventCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString    ( ProjectEvent::Command t )                                         { return Enum2WString( ProjectEventCommandMapping, t ); }

// ========================================================================= //
// ResponseEvent
// ========================================================================= //
const std::wstring RESPONSE_WSTRING                     = L"Response";

// ========================================================================= //
// InfoEvent
// ========================================================================= //

std::pair< InfoEvent::Command, const std::wstring > InfoEventCommandMapping[] = 
{
    std::make_pair( InfoEvent::Command::TreeStructure, L"TreeStructure" )
    , std::make_pair( InfoEvent::Command::MinimalTreeStructure, L"MinimalTreeStructure" )
    , std::make_pair( InfoEvent::Command::NodeInfo, L"NodeInfo" )
    , std::make_pair( InfoEvent::Command::PluginInfo, L"PluginInfo" )
    , std::make_pair( InfoEvent::Command::MinimalSceneInfo, L"MinimalSceneInfo" )

    , std::make_pair( InfoEvent::Command::ListSceneAssets, L"ListSceneAssets" )
    , std::make_pair( InfoEvent::Command::ListProjectNames, L"ListProjectNames" )
    , std::make_pair( InfoEvent::Command::ListScenes, L"ListScenes" )
    , std::make_pair( InfoEvent::Command::ListAssetsPaths, L"ListAssetsPaths" )
    , std::make_pair( InfoEvent::Command::ListCategoriesNames, L"ListCategoriesNames" )
    , std::make_pair( InfoEvent::Command::ListProjects, L"ListProjects" )
    , std::make_pair( InfoEvent::Command::ListAllScenes, L"ListAllScenes" )
    , std::make_pair( InfoEvent::Command::ListAllFolders, L"ListAllFolders" )
    , std::make_pair( InfoEvent::Command::ListResourcesInFolders, L"ListResourcesInFolders" )
    , std::make_pair( InfoEvent::Command::ListAllResources, L"ListAllResources" )

    , std::make_pair( InfoEvent::Command::Timelines, L"TimeLines" ) 
    , std::make_pair( InfoEvent::Command::ListTimelineKeyframes, L"ListTimelineKeyframes" ) 
    , std::make_pair( InfoEvent::Command::CheckTimelineTime, L"CheckTimelineTime" )

    , std::make_pair( InfoEvent::Command::Performance, L"Performance" )
    , std::make_pair( InfoEvent::Command::Videocards, L"VideoCards" )
    , std::make_pair( InfoEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> InfoEvent::Command WString2T     ( const std::wstring& s, const InfoEvent::Command& defaultVal )     { return WString2T( InfoEventCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString    ( InfoEvent::Command t )                                            { return Enum2WString( InfoEventCommandMapping, t ); }

// ========================================================================= //
// TimeLineEvent
// ========================================================================= //
const std::wstring TIMELINE_TYPE_WSTRING                = L"Type";
const std::wstring TIMELINE_TIME_VALUE_WSTRING          = L"Time";
const std::wstring TIMELINE_DURATION_VALUE_WSTRING      = L"Duration";
const std::wstring TIMELINE_NEW_NAME_WSTRING            = L"TimelineNewName";
const std::wstring TIMELINE_WRAP_METHOD_WSTRING			= L"WrapMethod";
const std::wstring SCENE_NAME_WSTRING                   = L"SceneName";

// Timeline Command

std::pair< TimeLineEvent::Command, const std::wstring > TimeLineEventCommandMapping[] = 
{
    std::make_pair( TimeLineEvent::Command::AddTimeline, L"AddTimeline" )
    , std::make_pair( TimeLineEvent::Command::DeleteTimeline, L"DeleteTimeline" ) 
    , std::make_pair( TimeLineEvent::Command::ForceDeleteTimeline, L"ForceDeleteTimeline" ) 
    , std::make_pair( TimeLineEvent::Command::RenameTimeline, L"RenameTimeline" )
    , std::make_pair( TimeLineEvent::Command::SetDuration, L"SetDuration" )
    , std::make_pair( TimeLineEvent::Command::SetWrapPreBehavior, L"SetWrapPreBehavior" ) 
    , std::make_pair( TimeLineEvent::Command::SetWrapPostBehavior, L"SetWrapPostBehavior" ) 
    , std::make_pair( TimeLineEvent::Command::Play, L"Play" )
    , std::make_pair( TimeLineEvent::Command::Stop, L"Stop" )
    , std::make_pair( TimeLineEvent::Command::PlayReverse, L"PlayReverse" ) 
    , std::make_pair( TimeLineEvent::Command::Goto, L"Goto" )
    , std::make_pair( TimeLineEvent::Command::GotoAndPlay, L"GotoAndPlay" )
    , std::make_pair( TimeLineEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> TimeLineEvent::Command WString2T     ( const std::wstring& s, const TimeLineEvent::Command& defaultVal )     { return WString2T( TimeLineEventCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString        ( TimeLineEvent::Command t )                                            { return Enum2WString( TimeLineEventCommandMapping, t ); }

// Timeline WrapMethod

std::pair< TimelineWrapMethod, const std::wstring > TimelineWrapMethodMapping[] = 
{
    std::make_pair( TimelineWrapMethod::TWM_CLAMP, L"Clamp" )
    , std::make_pair( TimelineWrapMethod::TWM_MIRROR, L"Mirror" ) 
    , std::make_pair( TimelineWrapMethod::TWM_REPEAT, L"Repeat" ) 
    , std::make_pair( TimelineWrapMethod::TWM_TOTAL, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> TimelineWrapMethod  WString2T    ( const std::wstring& s, const TimelineWrapMethod& defaultVal )     { return WString2T( TimelineWrapMethodMapping, s, defaultVal ); }
template<> const std::wstring& T2WString    ( TimelineWrapMethod t )                                            { return Enum2WString( TimelineWrapMethodMapping, t ); }


// Timeline Type

std::pair< TimelineType, const std::wstring > TimelineTypeMapping[] = 
{
    std::make_pair( TimelineType::TT_DEFAULT, L"Default" )
    , std::make_pair( TimelineType::TT_OFFSET, L"Offset" ) 
    , std::make_pair( TimelineType::TT_CONST, L"Const" ) 
    , std::make_pair( TimelineType::TT_TOTAL, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> TimelineType         WString2T    ( const std::wstring& s, const TimelineType& defaultVal )  { return WString2T( TimelineTypeMapping, s, defaultVal ); }
template<> const std::wstring&  T2WString    ( TimelineType t )                                         { return Enum2WString( TimelineTypeMapping, t ); }


// ========================================================================= //
// WidgetEvent
// ========================================================================= //
const std::wstring WIDGET_ACTION_WSTRING                = L"Action";
const std::wstring WIDGET_TIME_VALUE_WSTRING            = L"Time";

std::pair< WidgetEvent::Command, const std::wstring > WidgetEventCommandMapping[] = 
{
    std::make_pair( WidgetEvent::Command::Crawl, L"Crawl" )
    , std::make_pair( WidgetEvent::Command::Counter, L"Counter" ) 
    , std::make_pair( WidgetEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> WidgetEvent::Command WString2T       ( const std::wstring& s, const WidgetEvent::Command& defaultVal )   { return WString2T( WidgetEventCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString        ( WidgetEvent::Command t )                                          { return Enum2WString( WidgetEventCommandMapping, t ); }

// ========================================================================= //
// VideoCardEvent
// ========================================================================= //
const std::wstring VIDEO_CARD_NUMBER_WSTRING            = L"Number";
const std::wstring VIDEO_CARD_VALUE_WSTRING             = L"Value";
const std::wstring VIDEO_CARD_REFERENCE_MODE_WSTRING    = L"ReferenceMode";


std::pair< VideoCardEvent::Command, const std::wstring > VideoCardEventCommandMapping[] = 
{
    std::make_pair( VideoCardEvent::Command::EnableOutput, L"EnableOutput" )
    , std::make_pair( VideoCardEvent::Command::DisableOutput, L"DisableOutput" ) 
    , std::make_pair( VideoCardEvent::Command::EnableKey, L"EnableKey" ) 
    , std::make_pair( VideoCardEvent::Command::DisableKey, L"DisableKey" )
    , std::make_pair( VideoCardEvent::Command::ReferenceMode, L"ReferenceMode" )
    , std::make_pair( VideoCardEvent::Command::ReferenceOffsetH, L"ReferenceOffsetH" ) 
    , std::make_pair( VideoCardEvent::Command::ReferenceOffsetV, L"ReferenceOffsetV" ) 
    , std::make_pair( VideoCardEvent::Command::EnableInput, L"EnableInput" )
    , std::make_pair( VideoCardEvent::Command::DisableInput, L"DisableInput" )
    , std::make_pair( VideoCardEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> VideoCardEvent::Command WString2T    ( const std::wstring& s, const VideoCardEvent::Command& defaultVal )    { return WString2T( VideoCardEventCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString        ( VideoCardEvent::Command t )                                           { return Enum2WString( VideoCardEventCommandMapping, t ); }


std::pair< VideoCardEvent::VideoReferenceMode, const std::wstring > VideoCardVideoReferenceModeMapping[] = 
{
    std::make_pair( VideoCardEvent::VideoReferenceMode::FreeRun, L"FreeRun" )
    , std::make_pair( VideoCardEvent::VideoReferenceMode::AnalogBlackBurst, L"AnalogBlackBurst" ) 
    , std::make_pair( VideoCardEvent::VideoReferenceMode::AnalogTriLevel, L"AnalogTriLevel" ) 
    , std::make_pair( VideoCardEvent::VideoReferenceMode::DigitalInput1, L"DigitalInput1" )
    , std::make_pair( VideoCardEvent::VideoReferenceMode::DigitalInput2, L"DigitalInput2" )
    , std::make_pair( VideoCardEvent::VideoReferenceMode::FailMode, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> VideoCardEvent::VideoReferenceMode WString2T ( const std::wstring& s, const VideoCardEvent::VideoReferenceMode& defaultVal )     { return WString2T( VideoCardVideoReferenceModeMapping, s, defaultVal ); }
template<> const std::wstring& T2WString                ( VideoCardEvent::VideoReferenceMode t )                                            { return Enum2WString( VideoCardVideoReferenceModeMapping, t ); }


// ========================================================================= //
// TimerEvent
// ========================================================================= //
const std::wstring TIMER_HOURS_WSTRING                  = L"Hours";
const std::wstring TIMER_MINUTES_WSTRING                = L"Minutes";
const std::wstring TIMER_SECONDS_WSTRING                = L"Seconds";
const std::wstring TIMER_MILLISECONDS_WSTRING           = L"Milliseconds";

std::pair< TimerEvent::Command, const std::wstring > TimerEventCommandMapping[] = 
{
    std::make_pair( TimerEvent::Command::Start, L"Start" )
    , std::make_pair( TimerEvent::Command::Stop, L"Stop" ) 
    , std::make_pair( TimerEvent::Command::Reset, L"Reset" ) 
    , std::make_pair( TimerEvent::Command::SetTime, L"SetTime" )
    , std::make_pair( TimerEvent::Command::SetTimeStart, L"SetTimeStart" )
    , std::make_pair( TimerEvent::Command::SetTimeStop, L"SetTimeStop" ) 
    , std::make_pair( TimerEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> TimerEvent::Command WString2T    ( const std::wstring& s, const TimerEvent::Command& defaultVal )    { return WString2T( TimerEventCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString    ( TimerEvent::Command t )                                           { return Enum2WString( TimerEventCommandMapping, t ); }

// ========================================================================= //
// RenderingModeEvent
// ========================================================================= //
const std::wstring REQUESTED_FPS_WSTRING                = L"FPS";
const std::wstring NUM_FRAMES_WSTRING                   = L"NumberFrames";
const std::wstring RENDERING_FILE_PATH                  = L"FilePath";

std::pair< RenderingModeEvent::Command, const std::wstring > RenderingModeEventCommandMapping[] = 
{
    std::make_pair( RenderingModeEvent::Command::ScreenShot, L"ScreenShot" )
    , std::make_pair( RenderingModeEvent::Command::RenderOffscreen, L"RenderOffscreen" ) 
    , std::make_pair( RenderingModeEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> RenderingModeEvent::Command WString2T    ( const std::wstring& s, const RenderingModeEvent::Command& defaultVal )    { return WString2T( RenderingModeEventCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString            ( RenderingModeEvent::Command t )                                           { return Enum2WString( RenderingModeEventCommandMapping, t ); }

// ========================================================================= //
// GlobalEffectEvent
// ========================================================================= //
const std::wstring GLOBAL_EFFECT_NAME_WSTRING                = L"GlobalEffectName";

std::pair< GlobalEffectEvent::Command, const std::wstring > GlobalEffectEventCommandMapping[] = 
{
    std::make_pair( GlobalEffectEvent::Command::SetGlobalEffect, L"SetGlobalEffect" )
    , std::make_pair( GlobalEffectEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> GlobalEffectEvent::Command WString2T     ( const std::wstring& s, const GlobalEffectEvent::Command& defaultVal )     { return WString2T( GlobalEffectEventCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString            ( GlobalEffectEvent::Command t )                                            { return Enum2WString( GlobalEffectEventCommandMapping, t ); }


// ========================================================================= //
// TimelineKeyframeEvent
// ========================================================================= //
const std::wstring KEYFRAME_NAME_WSTRING                = L"KeyframeName";
const std::wstring KEYFRAME_INDEX_WSTRING               = L"KeyframeIndex";
const std::wstring NEW_KEYFRAME_TYPE_WSTRING            = L"NewKeyframeType";
const std::wstring JUMP_TO_TIME_WSTRING                 = L"JumpToTime";
const std::wstring TOTAL_LOOP_COUNT_WSTRING             = L"TotalLoopCount";

std::pair< TimelineKeyframeEvent::Command, const std::wstring > TimelineKeyframeEventCommandMapping[] = 
{
    std::make_pair( TimelineKeyframeEvent::Command::AddKeyframe, L"AddKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::Command::RemoveKeyframe, L"RemoveKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> TimelineKeyframeEvent::Command   WString2T   ( const std::wstring& s, const TimelineKeyframeEvent::Command& defaultVal )     { return WString2T( TimelineKeyframeEventCommandMapping, s, defaultVal ); }
template<> const std::wstring&              T2WString   ( TimelineKeyframeEvent::Command t )                                            { return Enum2WString( TimelineKeyframeEventCommandMapping, t ); }

std::pair< TimelineKeyframeEvent::KeyframeType, const std::wstring > KeyframeTypeMapping[] = 
{
    std::make_pair( TimelineKeyframeEvent::KeyframeType::StopKeyframe, L"StopKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::LoopReverseKeyframe, L"LoopReverseKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::LoopJumpKeyframe, L"LoopJumpKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::LoopRestartKeyframe, L"LoopRestartKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::NullKeyframe, L"NullKeyframe" )
    , std::make_pair( TimelineKeyframeEvent::KeyframeType::KeyframeTypeFail, SerializationHelper::EMPTY_WSTRING )      // default
};

template<> TimelineKeyframeEvent::KeyframeType  WString2T   ( const std::wstring& s, const TimelineKeyframeEvent::KeyframeType& defaultVal )    { return WString2T( KeyframeTypeMapping, s, defaultVal ); }
template<> const std::wstring&                  T2WString   ( TimelineKeyframeEvent::KeyframeType t )                                           { return Enum2WString( KeyframeTypeMapping, t ); }

// ========================================================================= //
// HightmapEvent
// ========================================================================= //
const std::wstring COMMAND_HM_ENABLE_WSTRING    = L"Enable";
const std::wstring COMMAND_HM_START_WSTRING     = L"Start";
const std::wstring COMMAND_HM_RESET_WSTRING     = L"Reset";
const std::wstring COMMAND_HM_SHOW_WSTRING      = L"Show";
const std::wstring COMMAND_HM_ZOOM_WSTRING      = L"Zoom";
const std::wstring COMMAND_HM_ZOOM_OUT_WSTRING  = L"ZoomOut";
const std::wstring COMMAND_HM_SET_WSTRING       = L"Set";
const std::wstring COMMAND_HM_ANIM_WSTRING      = L"Anim";
const std::wstring COMMAND_HM_ANIM2_WSTRING     = L"Anim2";
const std::wstring COMMAND_HM_SET3_WSTRING      = L"Set3";

std::pair< HightmapEvent::Command, const std::wstring > HightmapEventCommandMapping[] = 
{
    std::make_pair( HightmapEvent::Command::Enable, L"Enable" )
    , std::make_pair( HightmapEvent::Command::Start, L"Start" ) 
    , std::make_pair( HightmapEvent::Command::Reset, L"Reset" ) 
    , std::make_pair( HightmapEvent::Command::Show, L"Show" )
    , std::make_pair( HightmapEvent::Command::Zoom, L"Zoom" )
    , std::make_pair( HightmapEvent::Command::ZoomOut, L"ZoomOut" ) 
    , std::make_pair( HightmapEvent::Command::Set, L"Set" ) 
    , std::make_pair( HightmapEvent::Command::Anim, L"Anim" )
    , std::make_pair( HightmapEvent::Command::Anim2, L"Anim2" )
    , std::make_pair( HightmapEvent::Command::Set3, L"Set3" ) 
    , std::make_pair( HightmapEvent::Command::Fail, SerializationHelper::EMPTY_WSTRING )      // default
};


template<> HightmapEvent::Command WString2T     ( const std::wstring& s, const HightmapEvent::Command& defaultVal )     { return WString2T( HightmapEventCommandMapping, s, defaultVal ); }
template<> const std::wstring& T2WString        ( HightmapEvent::Command t )                                            { return Enum2WString( HightmapEventCommandMapping, t ); }

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


//******************* LoadAssetEvent *************

LoadAssetEvent::LoadAssetEvent         () 
{}

// *************************************
//
void                LoadAssetEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::NODE_NAME_WSTRING, toWString( NodeName ) );
    ser.SetAttribute( SerializationHelper::PLUGIN_NAME_WSTRING, toWString( PluginName ) );
    ser.SetAttribute( SerializationHelper::ASSET_DATA_WSTRING, toWString( AssetData ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_WSTRING, toWString( SceneName ) );
}

// *************************************
//
IEventPtr                LoadAssetEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        LoadAssetEventPtr newEvent  = std::make_shared<LoadAssetEvent>();
        newEvent->PluginName        = toString( deser.GetAttribute( SerializationHelper::PLUGIN_NAME_WSTRING ) );
        newEvent->NodeName          = toString( deser.GetAttribute( SerializationHelper::NODE_NAME_WSTRING ) );
        newEvent->SceneName         = toString( deser.GetAttribute( SerializationHelper::SCENE_NAME_WSTRING ) );
        newEvent->AssetData         = toString( deser.GetAttribute( SerializationHelper::ASSET_DATA_WSTRING ) );
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
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_WSTRING, toWString( SceneName ) );
    ser.SetAttribute( SerializationHelper::NODE_NAME_WSTRING, toWString( NodeName ) );
    ser.SetAttribute( SerializationHelper::PLUGIN_NAME_WSTRING, toWString( PluginName ) );
    ser.SetAttribute( SerializationHelper::PARAM_NAME_WSTRING, toWString( ParamName ) );
    ser.SetAttribute( SerializationHelper::PARAM_VALUE_WSTRING, Value );
    ser.SetAttribute( SerializationHelper::KEY_TIME_WSTRING, std::to_wstring( Time ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( ParamCommand ) );
    ser.SetAttribute( SerializationHelper::PARAM_TARGET_TYPE_WSTRING, SerializationHelper::T2WString( ParamTargetType ) );
}

// *************************************
//
IEventPtr           ParamKeyEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        ParamKeyEventPtr newEvent   = std::make_shared<ParamKeyEvent>();
        newEvent->SceneName         = toString( deser.GetAttribute( SerializationHelper::SCENE_NAME_WSTRING) );
        newEvent->PluginName        = toString( deser.GetAttribute( SerializationHelper::PLUGIN_NAME_WSTRING ) );
        newEvent->NodeName          = toString( deser.GetAttribute( SerializationHelper::NODE_NAME_WSTRING ) );
        newEvent->ParamName         = toString( deser.GetAttribute( SerializationHelper::PARAM_NAME_WSTRING ) );
        newEvent->Value             = deser.GetAttribute( SerializationHelper::PARAM_VALUE_WSTRING );
        newEvent->ParamCommand      = SerializationHelper::WString2T<ParamKeyEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), ParamKeyEvent::Command::Fail );
        newEvent->ParamTargetType   = SerializationHelper::WString2T<ParamKeyEvent::TargetType>( deser.GetAttribute( SerializationHelper::PARAM_TARGET_TYPE_WSTRING ), ParamKeyEvent::TargetType::FailTarget );
        newEvent->Time              = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::KEY_TIME_WSTRING ), std::numeric_limits<float>::quiet_NaN() );

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
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( AssetCommand ) );
}

// *************************************
//
IEventPtr                AssetEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        AssetEventPtr newEvent		= std::make_shared< AssetEvent >();
        newEvent->AssetCommand      = SerializationHelper::WString2T< AssetEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), AssetEvent::Command::Fail );
        
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
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_WSTRING, toWString( SceneName ) );
    ser.SetAttribute( SerializationHelper::NEW_SCENE_NAME_WSTRING, toWString( NewSceneName ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( SceneCommand ) );
    ser.SetAttribute( SerializationHelper::ATTACH_INDEX_WSTRING, toWString( AttachIndex ) );
}

// *************************************
//
IEventPtr                SceneEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        SceneEventPtr newEvent		= std::make_shared< SceneEvent >();
        newEvent->SceneName         = toString( deser.GetAttribute( SerializationHelper::SCENE_NAME_WSTRING ) );
        newEvent->NewSceneName      = toString( deser.GetAttribute( SerializationHelper::NEW_SCENE_NAME_WSTRING ) );
        newEvent->SceneCommand      = SerializationHelper::WString2T< SceneEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), SceneEvent::Command::Fail );
        newEvent->AttachIndex       = SerializationHelper::WString2T<unsigned int>( deser.GetAttribute( SerializationHelper::ATTACH_INDEX_WSTRING ), 0 );
        
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
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_WSTRING, toWString( SceneName ) );
    ser.SetAttribute( SerializationHelper::NODE_NAME_WSTRING, toWString( NodePath ) );
    ser.SetAttribute( SerializationHelper::NEW_NODE_NAME_WSTRING, toWString( NewNodeName ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( SceneCommand ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING, toWString( TimelinePath ) );
    ser.SetAttribute( SerializationHelper::REQUEST_WSTRING, toWString( Request ) );
    ser.SetAttribute( SerializationHelper::ATTACH_INDEX_WSTRING, toWString( AttachIndex ) );
}

// *************************************
//
IEventPtr                NodeStructureEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        NodeStructureEventPtr newEvent   = std::make_shared<NodeStructureEvent>();
        newEvent->SceneName         = toString( deser.GetAttribute( SerializationHelper::SCENE_NAME_WSTRING ) );
        newEvent->NodePath          = toString( deser.GetAttribute( SerializationHelper::NODE_NAME_WSTRING ) );
        newEvent->NewNodeName       = toString( deser.GetAttribute( SerializationHelper::NEW_NODE_NAME_WSTRING ) );
        newEvent->TimelinePath      = toString( deser.GetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING ) );
        newEvent->SceneCommand      = SerializationHelper::WString2T<NodeStructureEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), NodeStructureEvent::Command::Fail );
        newEvent->Request           = toString( deser.GetAttribute( SerializationHelper::REQUEST_WSTRING ) );
        newEvent->AttachIndex       = SerializationHelper::WString2T<unsigned int>( deser.GetAttribute( SerializationHelper::ATTACH_INDEX_WSTRING ), 0 );
        
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
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::NODE_NAME_WSTRING, toWString( NodePath ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_WSTRING, toWString( SceneName ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( PluginCommand ) );
    ser.SetAttribute( SerializationHelper::PLUGIN_NAME_WSTRING, toWString( PluginName ) );
    ser.SetAttribute( SerializationHelper::ATTACH_INDEX_WSTRING, toWString( AttachIndex ) );
    ser.SetAttribute( SerializationHelper::PLUGIN_UID_WSTRING, toWString( PluginUID ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING, toWString( TimelinePath ) );
	ser.SetAttribute( SerializationHelper::REQUEST_WSTRING, toWString( Request ) );
}

// *************************************
//
IEventPtr                PluginStructureEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        PluginStructureEventPtr newEvent   = std::make_shared<PluginStructureEvent>();
        newEvent->NodePath          = toString( deser.GetAttribute( SerializationHelper::NODE_NAME_WSTRING ) );
        newEvent->SceneName         = toString( deser.GetAttribute( SerializationHelper::SCENE_NAME_WSTRING ) );
        newEvent->PluginName        = toString( deser.GetAttribute( SerializationHelper::PLUGIN_NAME_WSTRING ) );
        newEvent->PluginCommand     = SerializationHelper::WString2T<PluginStructureEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), PluginStructureEvent::Command::Fail );
        newEvent->AttachIndex       = SerializationHelper::WString2T<unsigned int>( deser.GetAttribute( SerializationHelper::ATTACH_INDEX_WSTRING ), 0 );
        newEvent->PluginUID         = toString( deser.GetAttribute( SerializationHelper::PLUGIN_UID_WSTRING ) );
        newEvent->TimelinePath      = toString( deser.GetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING ) );
        newEvent->Request			= toString( deser.GetAttribute( SerializationHelper::REQUEST_WSTRING ) );

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
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::REQUEST_WSTRING, toWString( Request ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( ProjectCommand ) );
}

// *************************************
//
IEventPtr                ProjectEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        ProjectEventPtr newEvent            = std::make_shared<ProjectEvent>();
        newEvent->Request                   = toString( deser.GetAttribute( SerializationHelper::REQUEST_WSTRING ) );
        newEvent->ProjectCommand            = SerializationHelper::WString2T<ProjectEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), ProjectEvent::Command::Fail );
        
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
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::RESPONSE_WSTRING, Response );
}

// *************************************
//
IEventPtr                ResponseEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        ResponseEventPtr newEvent   = std::make_shared<ResponseEvent>();
        newEvent->Response          = deser.GetAttribute( SerializationHelper::RESPONSE_WSTRING );
        
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
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( InfoCommand ) );
    ser.SetAttribute( SerializationHelper::REQUEST_WSTRING, toWString( Request ) );
}

// *************************************
//
IEventPtr                InfoEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        InfoEventPtr newEvent   = std::make_shared<InfoEvent>();
        newEvent->InfoCommand   = SerializationHelper::WString2T<InfoEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), InfoEvent::Command::Fail );
        newEvent->Request       = toString( deser.GetAttribute( SerializationHelper::REQUEST_WSTRING ) );
        
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
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( TimelineCommand ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING, toWString( TimelineName ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_NEW_NAME_WSTRING, toWString( NewTimelineName ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_TYPE_WSTRING, SerializationHelper::T2WString( TimelineType ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_WSTRING, toWString( Time ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_DURATION_VALUE_WSTRING, toWString( Duration ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_WRAP_METHOD_WSTRING, SerializationHelper::T2WString( WrapMethod ) );
}

// *************************************
//
IEventPtr                TimeLineEvent::Create          ( IDeserializer & deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        TimeLineEventPtr newEvent   = std::make_shared< TimeLineEvent >();
        newEvent->Time              = SerializationHelper::WString2T< float >( deser.GetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_WSTRING ), std::numeric_limits<float>::quiet_NaN() );
        newEvent->TimelineCommand   = SerializationHelper::WString2T< TimeLineEvent::Command >( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), TimeLineEvent::Command::Fail );
        newEvent->TimelineName      = toString( deser.GetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING ) );
        newEvent->TimelineType      = SerializationHelper::WString2T< bv::TimelineType >( deser.GetAttribute( SerializationHelper::TIMELINE_TYPE_WSTRING ), bv::TimelineType::TT_OFFSET );
        newEvent->Duration          = SerializationHelper::WString2T< float >( deser.GetAttribute( SerializationHelper::TIMELINE_DURATION_VALUE_WSTRING ), 0.0f );
        newEvent->WrapMethod        = SerializationHelper::WString2T< TimelineWrapMethod >( deser.GetAttribute( SerializationHelper::TIMELINE_WRAP_METHOD_WSTRING ), TimelineWrapMethod::TWM_CLAMP );
        newEvent->NewTimelineName   = toString( deser.GetAttribute( SerializationHelper::TIMELINE_NEW_NAME_WSTRING ) );

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
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( TimerCommand ) );
    ser.SetAttribute( SerializationHelper::NODE_NAME_WSTRING, toWString( NodeName ) );
    ser.SetAttribute( SerializationHelper::TIMER_HOURS_WSTRING, toWString( Hours ) );
    ser.SetAttribute( SerializationHelper::TIMER_MINUTES_WSTRING, toWString( Minutes ) );
    ser.SetAttribute( SerializationHelper::TIMER_SECONDS_WSTRING, toWString( Seconds ) );
    ser.SetAttribute( SerializationHelper::TIMER_MILLISECONDS_WSTRING, toWString( Milliseconds ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_WSTRING, toWString( SceneName ) );
}

// *************************************
//
IEventPtr                TimerEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        TimerEventPtr newEvent      = std::make_shared<TimerEvent>();
        newEvent->TimerCommand      = SerializationHelper::WString2T<TimerEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), TimerEvent::Command::Fail );
        newEvent->NodeName          = toString( deser.GetAttribute( SerializationHelper::NODE_NAME_WSTRING ) );
        newEvent->SceneName         = toString( deser.GetAttribute( SerializationHelper::SCENE_NAME_WSTRING ) );
        newEvent->Hours             = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::TIMER_HOURS_WSTRING ), 0.0f );
        newEvent->Minutes           = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MINUTES_WSTRING ), 0.0f );
        newEvent->Seconds           = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::TIMER_SECONDS_WSTRING ), 0.0f );
        newEvent->Milliseconds      = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MILLISECONDS_WSTRING ), 0.0f );

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


//******************* WidgetEvent *************

// *************************************
//
void                WidgetEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( WidgetCommand ) );
    ser.SetAttribute( SerializationHelper::NODE_NAME_WSTRING, toWString( NodeName ) );
    ser.SetAttribute( SerializationHelper::WIDGET_ACTION_WSTRING, toWString( Action ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_WSTRING, toWString( SceneName ) );
}

// *************************************
//
IEventPtr                WidgetEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        WidgetEventPtr newEvent     = std::make_shared<WidgetEvent>();
        newEvent->WidgetCommand     = SerializationHelper::WString2T<WidgetEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), WidgetEvent::Command::Fail );
        newEvent->NodeName          = toString( deser.GetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING ) );
        newEvent->SceneName         = toString( deser.GetAttribute( SerializationHelper::SCENE_NAME_WSTRING ) );
        newEvent->Action            = toString( deser.GetAttribute( SerializationHelper::WIDGET_ACTION_WSTRING ) );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               WidgetEvent::Clone             () const
{   return IEventPtr( new WidgetEvent( *this ) );  }

// *************************************
//
EventType           WidgetEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        WidgetEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  WidgetEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           WidgetEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* VideoCardEvent *************

// *************************************
//
void                VideoCardEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( VideoCommand ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_NUMBER_WSTRING, toWString( Number ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_VALUE_WSTRING, toWString( Value ) );
    ser.SetAttribute( SerializationHelper::VIDEO_CARD_REFERENCE_MODE_WSTRING, SerializationHelper::T2WString( Mode ) );
}

// *************************************
//
IEventPtr                VideoCardEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        VideoCardEventPtr newEvent      = std::make_shared<VideoCardEvent>();
        newEvent->VideoCommand          = SerializationHelper::WString2T<VideoCardEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), VideoCardEvent::Command::Fail );
        newEvent->Mode                  = SerializationHelper::WString2T<VideoCardEvent::VideoReferenceMode>( deser.GetAttribute( SerializationHelper::VIDEO_CARD_REFERENCE_MODE_WSTRING ), VideoCardEvent::VideoReferenceMode::FailMode );
        newEvent->Value                 = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::VIDEO_CARD_VALUE_WSTRING ), 0.0f );
        newEvent->Number                = SerializationHelper::WString2T<int>( deser.GetAttribute( SerializationHelper::VIDEO_CARD_NUMBER_WSTRING ), 0 );

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


//******************* RenderingModeEvent *************

// *************************************
//
void                RenderingModeEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::REQUESTED_FPS_WSTRING, toWString( FPS ) );
    ser.SetAttribute( SerializationHelper::RENDERING_FILE_PATH, toWString( FilePath ) );
    ser.SetAttribute( SerializationHelper::NUM_FRAMES_WSTRING, toWString( NumFrames ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( RenderingCommand ) );
}

// *************************************
//
IEventPtr                RenderingModeEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        RenderingModeEventPtr newEvent  = std::make_shared<RenderingModeEvent>();
        newEvent->FilePath              = toString( deser.GetAttribute( SerializationHelper::RENDERING_FILE_PATH ) );
        newEvent->FPS                   = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::REQUESTED_FPS_WSTRING ), 60 );
        newEvent->NumFrames             = SerializationHelper::WString2T<int>( deser.GetAttribute( SerializationHelper::NUM_FRAMES_WSTRING ), 0 );
        newEvent->RenderingCommand      = SerializationHelper::WString2T<RenderingModeEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), RenderingModeEvent::Command::Fail );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               RenderingModeEvent::Clone             () const
{   return IEventPtr( new RenderingModeEvent( *this ) );  }

// *************************************
//
EventType           RenderingModeEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        RenderingModeEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  RenderingModeEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           RenderingModeEvent::GetEventType() const
{   return this->m_sEventType; }


//******************* GlobalEffectEvent *************

// *************************************
//
void                GlobalEffectEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::SCENE_NAME_WSTRING, toWString( SceneName ) );
    ser.SetAttribute( SerializationHelper::NODE_NAME_WSTRING, toWString( NodePath ) );
    ser.SetAttribute( SerializationHelper::GLOBAL_EFFECT_NAME_WSTRING, toWString( EffectName) );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING, toWString( TimelinePath ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( EffectCommand ) );
}

// *************************************
//
IEventPtr                GlobalEffectEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        GlobalEffectEventPtr newEvent   = std::make_shared<GlobalEffectEvent>();
        newEvent->NodePath              = toString( deser.GetAttribute( SerializationHelper::NODE_NAME_WSTRING ) );
        newEvent->SceneName             = toString( deser.GetAttribute( SerializationHelper::SCENE_NAME_WSTRING ) );
        newEvent->EffectName            = toString( deser.GetAttribute( SerializationHelper::GLOBAL_EFFECT_NAME_WSTRING ) );
        newEvent->TimelinePath          = toString( deser.GetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING ) );
        newEvent->EffectCommand         = SerializationHelper::WString2T<GlobalEffectEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), GlobalEffectEvent::Command::Fail );

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
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING, toWString( TimelinePath ) );
    ser.SetAttribute( SerializationHelper::KEYFRAME_NAME_WSTRING, toWString( KeyframeName ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( KeyframeCommand ) );
    ser.SetAttribute( SerializationHelper::NEW_KEYFRAME_TYPE_WSTRING, SerializationHelper::T2WString( NewKeyframeType ) );
    ser.SetAttribute( SerializationHelper::KEYFRAME_INDEX_WSTRING, toWString( KeyframeIndex ) );

    ser.SetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_WSTRING, toWString( Time ) );
    ser.SetAttribute( SerializationHelper::JUMP_TO_TIME_WSTRING, toWString( JumpToTime ) );
    ser.SetAttribute( SerializationHelper::TOTAL_LOOP_COUNT_WSTRING, toWString( TotalLoopCount ) );
}

// *************************************
//
IEventPtr                TimelineKeyframeEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        TimelineKeyframeEventPtr newEvent   = std::make_shared<TimelineKeyframeEvent>();
        newEvent->TimelinePath          = toString( deser.GetAttribute( SerializationHelper::TIMELINE_NAME_WSTRING ) );
        newEvent->KeyframeName          = toString( deser.GetAttribute( SerializationHelper::KEYFRAME_NAME_WSTRING ) );
        newEvent->KeyframeCommand       = SerializationHelper::WString2T<TimelineKeyframeEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), TimelineKeyframeEvent::Command::Fail );
        newEvent->NewKeyframeType       = SerializationHelper::WString2T<TimelineKeyframeEvent::KeyframeType>( deser.GetAttribute( SerializationHelper::NEW_KEYFRAME_TYPE_WSTRING ), TimelineKeyframeEvent::KeyframeType::KeyframeTypeFail );
        newEvent->KeyframeIndex         = SerializationHelper::WString2T( deser.GetAttribute( SerializationHelper::KEYFRAME_INDEX_WSTRING ), -1 );

        newEvent->Time                  = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::TIMELINE_TIME_VALUE_WSTRING ), std::numeric_limits<float>::quiet_NaN() );
        newEvent->JumpToTime            = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::JUMP_TO_TIME_WSTRING ), std::numeric_limits<float>::quiet_NaN() );
        newEvent->TotalLoopCount        = SerializationHelper::WString2T<unsigned int>( deser.GetAttribute( SerializationHelper::TOTAL_LOOP_COUNT_WSTRING ), 0 );

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


//******************* HightmapEvent *************

// *************************************
//
void                HightmapEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( SerializationHelper::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( SerializationHelper::COMMAND_WSTRING, SerializationHelper::T2WString( HightmapCommand ) );
    ser.SetAttribute( SerializationHelper::TIMER_HOURS_WSTRING, toWString( Hours ) );
    ser.SetAttribute( SerializationHelper::TIMER_MINUTES_WSTRING, toWString( Minutes ) );
    ser.SetAttribute( SerializationHelper::TIMER_SECONDS_WSTRING, toWString( Seconds ) );
    ser.SetAttribute( SerializationHelper::TIMER_MILLISECONDS_WSTRING, toWString( Milliseconds ) );
}

// *************************************
//
IEventPtr                HightmapEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( SerializationHelper::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        HightmapEventPtr newEvent   = std::make_shared<HightmapEvent>();
        newEvent->HightmapCommand   = SerializationHelper::WString2T<HightmapEvent::Command>( deser.GetAttribute( SerializationHelper::COMMAND_WSTRING ), HightmapEvent::Command::Fail );
        newEvent->Hours             = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::TIMER_HOURS_WSTRING ), 0.0f );
        newEvent->Minutes           = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MINUTES_WSTRING ), 0.0f );
        newEvent->Seconds           = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::TIMER_SECONDS_WSTRING ), 0.0f );
        newEvent->Milliseconds      = SerializationHelper::WString2T<float>( deser.GetAttribute( SerializationHelper::TIMER_MILLISECONDS_WSTRING ), 0.0f );

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
