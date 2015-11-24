#include "Events.h"

#include <cassert>

#pragma warning( push )
#pragma warning(disable:4100)

#include "Serialization/ISerializer.h"
#include "Serialization/IDeserializer.h"
#include "EventHelpers.h"

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



// ************************************* new Events *****************************************
const EventType LoadAssetEvent::m_sEventType        = 0x30000013;
std::string LoadAssetEvent::m_sEventName            = "LoadAssetEvent";

const EventType ParamKeyEvent::m_sEventType         = 0x30000006;
std::string ParamKeyEvent::m_sEventName             = "ParamKeyEvent";

const EventType NodeStructureEvent::m_sEventType    = 0x30000012;
std::string NodeStructureEvent::m_sEventName        = "NodeStructureEvent";

const EventType ProjectEvent::m_sEventType          = 0x30000013;
std::string ProjectEvent::m_sEventName              = "ProjectStructureEvent";

const EventType ResponseEvent::m_sEventType         = 0x30000008;
std::string ResponseEvent::m_sEventName             = "ResponseEvent";

const EventType InfoEvent::m_sEventType          = 0x30000007;
std::string InfoEvent::m_sEventName              = "InfoEvent";

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

// ************************************* Events Serialization *****************************************

namespace Serial
{
// Key names used to serialization.

const std::wstring EMPTY_WSTRING            = L"";

const std::wstring EVENT_TYPE_WSTRING       = L"Event";
const std::wstring NODE_NAME_WSTRING        = L"NodeName";
const std::wstring PLUGIN_NAME_WSTRING      = L"PluginName";
const std::wstring COMMAND_WSTRING          = L"Command";

// LoadAssetEvent
const std::wstring ASSET_DATA_WSTRING       = L"AssetData";

// ParamKeyEvent
const std::wstring PARAM_NAME_WSTRING       = L"ParamName";
const std::wstring PARAM_VALUE_WSTRING      = L"ParamValue";
const std::wstring KEY_TIME_WSTRING         = L"Time";

const std::wstring COMMAND_ADD_KEY_WSTRING  = L"AddKey";
const std::wstring COMMAND_REMOVE_KEY_WSTRING   = L"RemoveKey";
const std::wstring COMMAND_UPDATE_KEY_WSTRING   = L"UpdateKey";

// NodeStructureEvent
const std::wstring NEW_NODE_NAME_WSTRING        = L"NewNodeName";

const std::wstring COMMAND_ADD_NODE_WSTRING         = L"AddNode";
const std::wstring COMMAND_REMOVE_NODE_WSTRING      = L"RemoveNode";
const std::wstring COMMAND_ATTACH_PLUGIN_WSTRING    = L"AttachPlugin";
const std::wstring COMMAND_DETACH_PLUGIN_WSTRING    = L"DetachPlugin";
const std::wstring COMMAND_SET_NODE_VISIBLE_WSTRING     = L"SetNodeVisible";
const std::wstring COMMAND_SET_NODE_INVISIBLE_WSTRING   = L"SetNodeInvisible";

// ProjectEvent
const std::wstring REQUEST_WSTRING                          = L"Request";

const std::wstring COMMAND_SAVE_SCENE_WSTRING               = L"SaveScene";
const std::wstring COMMAND_LOAD_PROJECT_WSTRING             = L"LoadProject";
const std::wstring COMMAND_NEW_PROJECT_WSTRING              = L"NewProject";
const std::wstring COMMAND_SET_CURRENT_PROJECT_WSTRING      = L"SetCurrentProject";
const std::wstring COMMAND_LIST_PROJECTS_NAMES_WSTRING      = L"ListProjectNames";
const std::wstring COMMAND_LIST_SCENES_WSTRING              = L"ListScenes";
const std::wstring COMMAND_LIST_ASSETS_PATHS_WSTRING        = L"ListAssetsPaths";
const std::wstring COMMAND_LIST_CATEGORIES_NAMES_WSTRING    = L"ListCategoriesNames";
const std::wstring COMMAND_LIST_PROJECTS_WSTRING            = L"ListProjects";

// ResponseEvent
const std::wstring RESPONSE_WSTRING                     = L"Response";

// InfoEvent
const std::wstring COMMAND_TREE_STRUCTURE_WSTRING       = L"TreeStructure";
const std::wstring COMMAND_PERFORMANCE_WSTRING          = L"Performance";
const std::wstring COMMAND_TIMELINES_WSTRING            = L"TimeLines";
const std::wstring COMMAND_NODE_INFO_WSTRING            = L"NodeInfo";
const std::wstring COMMAND_VIDEO_CARDS_WSTRING          = L"VideoCards";

// TimeLineEvent
const std::wstring COMMAND_PLAY_WSTRING                 = L"Play";
const std::wstring COMMAND_STOP_WSTRING                 = L"Stop";
const std::wstring COMMAND_PLAY_REVERSE_WSTRING         = L"PlayReverse";
const std::wstring COMMAND_GOTO_WSTRING                 = L"Goto";
const std::wstring COMMAND_GOTO_AND_PLAY_WSTRING        = L"GotoAndPlay";

const std::wstring TIMELINE_TIME_VALUE_WSTRING          = L"Time";
const std::wstring TIMELINE_NAME_WSTRING                = L"TimeLineName";
const std::wstring SCENE_NAME_WSTRING                   = L"SceneName";

// WidgetEvent
const std::wstring COMMAND_CRAWL_WSTRING                = L"Crawl";
const std::wstring COMMAND_COUNTER_WSTRING              = L"Counter";

const std::wstring WIDGET_ACTION_WSTRING                = L"Action";
const std::wstring WIDGET_TIME_VALUE_WSTRING            = L"Time";

// VideoCardEvent
const std::wstring VIDEO_CARD_ACTION_WSTRING            = L"Action";

const std::wstring COMMAND_VIDEO_CARD_OFF_WSTRING       = L"Off";
const std::wstring COMMAND_VIDEO_CARD_ON_WSTRING        = L"On";
const std::wstring COMMAND_VIDEO_CARD_KEY_OFF_WSTRING   = L"KeyOff";
const std::wstring COMMAND_VIDEO_CARD_KEY_ON_WSTRING    = L"KeyOn";

// TimerEvent
const std::wstring TIMER_HOURS_WSTRING                  = L"Hours";
const std::wstring TIMER_MINUTES_WSTRING                = L"Minutes";
const std::wstring TIMER_SECONDS_WSTRING                = L"Seconds";
const std::wstring TIMER_MILLISECONDS_WSTRING           = L"Milliseconds";

const std::wstring COMMAND_START_TIMER_WSTRING          = L"Start";
const std::wstring COMMAND_STOP_TIMER_WSTRING           = L"Stop";
const std::wstring COMMAND_RESET_TIMER_WSTRING          = L"Reset";
const std::wstring COMMAND_SET_TIME_WSTRING             = L"SetTime";
const std::wstring COMMAND_SET_TIME_START_WSTRING       = L"SetTimeStart";
const std::wstring COMMAND_SET_TIME_STOP_WSTRING        = L"SetTimeStop";

// HightmapEvent
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
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::NODE_NAME_WSTRING, toWString( NodeName ) );
    ser.SetAttribute( Serial::PLUGIN_NAME_WSTRING, toWString( PluginName ) );
    ser.SetAttribute( Serial::ASSET_DATA_WSTRING, toWString( AssetData ) );
}

// *************************************
//
IEventPtr                LoadAssetEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        LoadAssetEventPtr newEvent  = std::make_shared<LoadAssetEvent>();
        newEvent->PluginName        = toString( deser.GetAttribute( Serial::PLUGIN_NAME_WSTRING ) );
        newEvent->NodeName          = toString( deser.GetAttribute( Serial::NODE_NAME_WSTRING ) );
        newEvent->AssetData         = toString( deser.GetAttribute( Serial::ASSET_DATA_WSTRING ) );
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

// ******************************************************************************************
// ************************************* new Events *****************************************
// ******************************************************************************************


//******************* ParamKeyEvent *************

// *************************************
//
void                ParamKeyEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::SCENE_NAME_WSTRING, toWString( SceneName ) );
    ser.SetAttribute( Serial::NODE_NAME_WSTRING, toWString( NodeName ) );
    ser.SetAttribute( Serial::PLUGIN_NAME_WSTRING, toWString( PluginName ) );
    ser.SetAttribute( Serial::PARAM_NAME_WSTRING, toWString( ParamName ) );
    ser.SetAttribute( Serial::PARAM_VALUE_WSTRING, Value );
    ser.SetAttribute( Serial::KEY_TIME_WSTRING, std::to_wstring( Time ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( KeyCommand ) );
}

// *************************************
//
IEventPtr           ParamKeyEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        ParamKeyEventPtr newEvent   = std::make_shared<ParamKeyEvent>();
        newEvent->SceneName         = toString( deser.GetAttribute( Serial::SCENE_NAME_WSTRING) );
        newEvent->PluginName        = toString( deser.GetAttribute( Serial::PLUGIN_NAME_WSTRING ) );
        newEvent->NodeName          = toString( deser.GetAttribute( Serial::NODE_NAME_WSTRING ) );
        newEvent->ParamName         = toString( deser.GetAttribute( Serial::PARAM_NAME_WSTRING ) );
        newEvent->Value             = deser.GetAttribute( Serial::PARAM_VALUE_WSTRING );
        newEvent->Time              = stof( deser.GetAttribute( Serial::KEY_TIME_WSTRING ) );
        newEvent->KeyCommand        = WStringToCommand( deser.GetAttribute( Serial::COMMAND_WSTRING ) );
        
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
std::wstring ParamKeyEvent::CommandToWString    ( Command cmd )
{
    if( cmd == Command::AddKey )
        return Serial::COMMAND_ADD_KEY_WSTRING;
    else if( cmd == Command::RemoveKey )
        return Serial::COMMAND_REMOVE_KEY_WSTRING;
    else if( cmd == Command::UpdateKey )
        return Serial::COMMAND_UPDATE_KEY_WSTRING;
    else
        return Serial::EMPTY_WSTRING;     // No way to be here. warning: not all control paths return value
}
// *************************************
//
ParamKeyEvent::Command ParamKeyEvent::WStringToCommand    ( const std::wstring& string )
{
    if( string == Serial::COMMAND_ADD_KEY_WSTRING )
        return Command::AddKey;
    else if( string == Serial::COMMAND_REMOVE_KEY_WSTRING )
        return Command::RemoveKey;
    else if( string == Serial::COMMAND_UPDATE_KEY_WSTRING )
        return Command::UpdateKey;
    else
        return Command::Fail;
}

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





//******************* NodeStructureEvent *************

// *************************************
//
void                NodeStructureEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::NODE_NAME_WSTRING, toWString( NodeName ) );
    ser.SetAttribute( Serial::NEW_NODE_NAME_WSTRING, toWString( NewNodeName ) );
    ser.SetAttribute( Serial::PLUGIN_NAME_WSTRING, toWString( PluginName ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( SceneCommand ) );
}

// *************************************
//
IEventPtr                NodeStructureEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        NodeStructureEventPtr newEvent   = std::make_shared<NodeStructureEvent>();
        newEvent->NodeName          = toString( deser.GetAttribute( Serial::NODE_NAME_WSTRING ) );
        newEvent->NewNodeName       = toString( deser.GetAttribute( Serial::NEW_NODE_NAME_WSTRING ) );
        newEvent->PluginName        = toString( deser.GetAttribute( Serial::PLUGIN_NAME_WSTRING ) );
        newEvent->SceneCommand      = WStringToCommand( deser.GetAttribute( Serial::COMMAND_WSTRING ) );
        
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

// *************************************
//
std::wstring NodeStructureEvent::CommandToWString    ( Command cmd )
{
    if( cmd == Command::AddNode )
        return Serial::COMMAND_ADD_NODE_WSTRING;
    else if( cmd == Command::RemoveNode )
        return Serial::COMMAND_REMOVE_NODE_WSTRING;
    else if( cmd == Command::AttachPlugin )
        return Serial::COMMAND_ATTACH_PLUGIN_WSTRING;
    else if( cmd == Command::DetachPlugin )
        return Serial::COMMAND_DETACH_PLUGIN_WSTRING;
    else if( cmd == Command::SetNodeVisible )
        return Serial::COMMAND_SET_NODE_VISIBLE_WSTRING;
    else if( cmd == Command::SetNodeInvisible )
        return Serial::COMMAND_SET_NODE_INVISIBLE_WSTRING;
    else
        return Serial::EMPTY_WSTRING;     // No way to be here. warning: not all control paths return value
}
// *************************************
//
NodeStructureEvent::Command NodeStructureEvent::WStringToCommand    ( const std::wstring& string )
{
    if( string == Serial::COMMAND_ADD_NODE_WSTRING )
        return Command::AddNode;
    else if( string == Serial::COMMAND_REMOVE_NODE_WSTRING )
        return Command::RemoveNode;
    else if( string == Serial::COMMAND_ATTACH_PLUGIN_WSTRING)
        return Command::AttachPlugin;
    else if( string == Serial::COMMAND_DETACH_PLUGIN_WSTRING )
        return Command::DetachPlugin;
    else if( string == Serial::COMMAND_SET_NODE_VISIBLE_WSTRING )
        return Command::SetNodeVisible;
    else if( string == Serial::COMMAND_SET_NODE_INVISIBLE_WSTRING )
        return Command::SetNodeInvisible;
    else
        return Command::Fail;
}


//******************* ProjectEvent *************

// *************************************
//
void                ProjectEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::REQUEST_WSTRING, toWString( Request ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( ProjectCommand ) );
}

// *************************************
//
IEventPtr                ProjectEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        ProjectEventPtr newEvent            = std::make_shared<ProjectEvent>();
        newEvent->Request                   = toString( deser.GetAttribute( Serial::REQUEST_WSTRING ) );
        newEvent->ProjectCommand            = WStringToCommand( deser.GetAttribute( Serial::COMMAND_WSTRING ) );
        
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

// *************************************
//
std::wstring ProjectEvent::CommandToWString    ( Command cmd )
{
    if( cmd == Command::NewProject )
        return Serial::COMMAND_NEW_PROJECT_WSTRING;
    else if( cmd == Command::SetCurrentProject )
        return Serial::COMMAND_SET_CURRENT_PROJECT_WSTRING;
    else if( cmd == Command::ListAssetsPaths )
        return Serial::COMMAND_LIST_ASSETS_PATHS_WSTRING;
    else if( cmd == Command::ListCategoriesNames )
        return Serial::COMMAND_LIST_CATEGORIES_NAMES_WSTRING;
    else if( cmd == Command::ListProjectNames )
        return Serial::COMMAND_LIST_PROJECTS_NAMES_WSTRING;
    else if( cmd == Command::ListProjects )
        return Serial::COMMAND_LIST_PROJECTS_WSTRING;
    else if( cmd == Command::ListScenes )
        return Serial::COMMAND_LIST_SCENES_WSTRING;
    else if( cmd == Command::SaveScene )
        return Serial::COMMAND_SAVE_SCENE_WSTRING;
    else if( cmd == Command::LoadProject )
        return Serial::COMMAND_LOAD_PROJECT_WSTRING;
    else
        return Serial::EMPTY_WSTRING;     // No way to be here. warning: not all control paths return value
}
// *************************************
//
ProjectEvent::Command ProjectEvent::WStringToCommand    ( const std::wstring& string )
{
    if( string == Serial::COMMAND_NEW_PROJECT_WSTRING )
        return Command::NewProject;
    else if( string == Serial::COMMAND_SET_CURRENT_PROJECT_WSTRING )
        return Command::SetCurrentProject;
    else if( string == Serial::COMMAND_LIST_ASSETS_PATHS_WSTRING)
        return Command::ListAssetsPaths;
    else if( string == Serial::COMMAND_LIST_CATEGORIES_NAMES_WSTRING )
        return Command::ListCategoriesNames;
    else if( string == Serial::COMMAND_LIST_PROJECTS_NAMES_WSTRING )
        return Command::ListProjectNames;
    else if( string == Serial::COMMAND_LIST_PROJECTS_WSTRING )
        return Command::ListProjects;
    else if( string == Serial::COMMAND_LIST_SCENES_WSTRING )
        return Command::ListScenes;
    else if( string == Serial::COMMAND_SAVE_SCENE_WSTRING )
        return Command::SaveScene;
    else if( string == Serial::COMMAND_LOAD_PROJECT_WSTRING )
        return Command::LoadProject;
    else
        return Command::Fail;
}



//******************* ResponseEvent *************

// *************************************
//
void                ResponseEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::RESPONSE_WSTRING, Response );
}

// *************************************
//
IEventPtr                ResponseEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        ResponseEventPtr newEvent   = std::make_shared<ResponseEvent>();
        newEvent->Response          = deser.GetAttribute( Serial::RESPONSE_WSTRING );
        
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
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( InfoRequest ) );
    ser.SetAttribute( Serial::NODE_NAME_WSTRING, toWString( NodeName ) );
}

// *************************************
//
IEventPtr                InfoEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        InfoEventPtr newEvent    = std::make_shared<InfoEvent>();
        newEvent->NodeName          = toString( deser.GetAttribute( Serial::NODE_NAME_WSTRING ) );
        newEvent->InfoRequest       = WStringToCommand( deser.GetAttribute( Serial::COMMAND_WSTRING ) );
        
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

// *************************************
//
std::wstring InfoEvent::CommandToWString    ( Command cmd )
{
    if( cmd == Command::NodeInfo )
        return Serial::COMMAND_NODE_INFO_WSTRING;
    else if( cmd == Command::Performance )
        return Serial::COMMAND_PERFORMANCE_WSTRING;
    else if( cmd == Command::Timelines )
        return Serial::COMMAND_TIMELINES_WSTRING;
    else if( cmd == Command::TreeStructure )
        return Serial::COMMAND_TREE_STRUCTURE_WSTRING;
    else if( cmd == Command::Videocards )
        return Serial::COMMAND_VIDEO_CARDS_WSTRING;
    else
        return Serial::EMPTY_WSTRING;     // No way to be here. warning: not all control paths return value
}

// *************************************
//
InfoEvent::Command InfoEvent::WStringToCommand    ( const std::wstring& string )
{
    if( string == Serial::COMMAND_NODE_INFO_WSTRING )
        return Command::NodeInfo;
    else if( string == Serial::COMMAND_PERFORMANCE_WSTRING )
        return Command::Performance;
    else if( string == Serial::COMMAND_TIMELINES_WSTRING)
        return Command::Timelines;
    else if( string == Serial::COMMAND_TREE_STRUCTURE_WSTRING )
        return Command::TreeStructure;
    else if( string == Serial::COMMAND_VIDEO_CARDS_WSTRING )
        return Command::Videocards;
    else
        return Command::Fail;
}

//******************* TimeLineEvent *************

// *************************************
//
void                TimeLineEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( TimelineCommand ) );
    ser.SetAttribute( Serial::TIMELINE_NAME_WSTRING, toWString( TimelineName ) );
    ser.SetAttribute( Serial::SCENE_NAME_WSTRING, toWString( SceneName ) );
    ser.SetAttribute( Serial::TIMELINE_TIME_VALUE_WSTRING, toWString( Time ) );
}

// *************************************
//
IEventPtr                TimeLineEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        TimeLineEventPtr newEvent   = std::make_shared<TimeLineEvent>();
        newEvent->Time              = stof( deser.GetAttribute( Serial::TIMELINE_TIME_VALUE_WSTRING ) );
        newEvent->TimelineCommand   = WStringToCommand( deser.GetAttribute( Serial::COMMAND_WSTRING ) );
        newEvent->TimelineName      = toString( deser.GetAttribute( Serial::TIMELINE_NAME_WSTRING ) );
        newEvent->SceneName         = toString( deser.GetAttribute( Serial::SCENE_NAME_WSTRING ) );

        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               TimeLineEvent::Clone             () const
{   return IEventPtr( new TimeLineEvent( *this ) );  }

// *************************************
//
EventType           TimeLineEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        TimeLineEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  TimeLineEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           TimeLineEvent::GetEventType() const
{   return this->m_sEventType; }

// *************************************
//
std::wstring TimeLineEvent::CommandToWString    ( Command cmd )
{
    if( cmd == Command::Goto )
        return Serial::COMMAND_GOTO_WSTRING;
    else if( cmd == Command::GotoAndPlay )
        return Serial::COMMAND_GOTO_AND_PLAY_WSTRING;
    else if( cmd == Command::Play )
        return Serial::COMMAND_PLAY_WSTRING;
    else if( cmd == Command::PlayReverse )
        return Serial::COMMAND_PLAY_REVERSE_WSTRING;
    else if( cmd == Command::Stop )
        return Serial::COMMAND_STOP_WSTRING;
    else
        return Serial::EMPTY_WSTRING;     // No way to be here. warning: not all control paths return value
}

// *************************************
//
TimeLineEvent::Command TimeLineEvent::WStringToCommand    ( const std::wstring& string )
{
    if( string == Serial::COMMAND_GOTO_WSTRING )
        return Command::Goto;
    else if( string == Serial::COMMAND_GOTO_AND_PLAY_WSTRING )
        return Command::GotoAndPlay;
    else if( string == Serial::COMMAND_PLAY_WSTRING)
        return Command::Play;
    else if( string == Serial::COMMAND_PLAY_REVERSE_WSTRING )
        return Command::PlayReverse;
    else if( string == Serial::COMMAND_STOP_WSTRING )
        return Command::Stop;
    else
        return Command::Fail;
}

//******************* TimerEvent *************

// *************************************
//
void                TimerEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( TimerCommand ) );
    ser.SetAttribute( Serial::NODE_NAME_WSTRING, toWString( NodeName ) );
    ser.SetAttribute( Serial::TIMER_HOURS_WSTRING, toWString( Hours ) );
    ser.SetAttribute( Serial::TIMER_MINUTES_WSTRING, toWString( Minutes ) );
    ser.SetAttribute( Serial::TIMER_SECONDS_WSTRING, toWString( Seconds ) );
    ser.SetAttribute( Serial::TIMER_MILLISECONDS_WSTRING, toWString( Milliseconds ) );
}

// *************************************
//
IEventPtr                TimerEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        TimerEventPtr newEvent      = std::make_shared<TimerEvent>();
        newEvent->TimerCommand      = WStringToCommand( deser.GetAttribute( Serial::COMMAND_WSTRING ) );
        newEvent->NodeName          = toString( deser.GetAttribute( Serial::NODE_NAME_WSTRING ) );
        newEvent->Hours             = stof( deser.GetAttribute( Serial::TIMER_HOURS_WSTRING ) );
        newEvent->Minutes           = stof( deser.GetAttribute( Serial::TIMER_MINUTES_WSTRING ) );
        newEvent->Seconds           = stof( deser.GetAttribute( Serial::TIMER_SECONDS_WSTRING ) );
        newEvent->Milliseconds      = stof( deser.GetAttribute( Serial::TIMER_MILLISECONDS_WSTRING ) );

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

// *************************************
//
std::wstring TimerEvent::CommandToWString    ( Command cmd )
{
    if( cmd == Command::Start )
        return Serial::COMMAND_START_TIMER_WSTRING;
    if( cmd == Command::Stop )
        return Serial::COMMAND_STOP_TIMER_WSTRING;
    if( cmd == Command::Reset )
        return Serial::COMMAND_RESET_TIMER_WSTRING;
    if( cmd == Command::SetTime )
        return Serial::COMMAND_SET_TIME_WSTRING;
    if( cmd == Command::SetTimeStart )
        return Serial::COMMAND_SET_TIME_START_WSTRING;
    if( cmd == Command::SetTimeStop )
        return Serial::COMMAND_SET_TIME_STOP_WSTRING;
    return Serial::EMPTY_WSTRING;     // No way to be here. warning: not all control paths return value
}

// *************************************
//
TimerEvent::Command TimerEvent::WStringToCommand    ( const std::wstring& string )
{
    if( string == Serial::COMMAND_START_TIMER_WSTRING )
        return Command::Start;
    else if( string == Serial::COMMAND_STOP_TIMER_WSTRING )
        return Command::Stop;
    else if( string == Serial::COMMAND_RESET_TIMER_WSTRING )
        return Command::Reset;
    else if( string == Serial::COMMAND_SET_TIME_WSTRING )
        return Command::SetTime;
    else if( string == Serial::COMMAND_SET_TIME_START_WSTRING )
        return Command::SetTimeStart;
    else if( string == Serial::COMMAND_SET_TIME_STOP_WSTRING )
        return Command::SetTimeStop;
    return Command::Fail;
}

//******************* WidgetEvent *************

// *************************************
//
void                WidgetEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( WidgetCommand ) );
    ser.SetAttribute( Serial::TIMELINE_TIME_VALUE_WSTRING, toWString( Time ) );
    ser.SetAttribute( Serial::NODE_NAME_WSTRING, toWString( NodeName ) );
    ser.SetAttribute( Serial::WIDGET_ACTION_WSTRING, toWString( Action ) );
}

// *************************************
//
IEventPtr                WidgetEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        WidgetEventPtr newEvent     = std::make_shared<WidgetEvent>();
        newEvent->Time              = stof( deser.GetAttribute( Serial::WIDGET_TIME_VALUE_WSTRING ) );
        newEvent->WidgetCommand     = WStringToCommand( deser.GetAttribute( Serial::COMMAND_WSTRING ) );
        newEvent->NodeName          = toString( deser.GetAttribute( Serial::TIMELINE_NAME_WSTRING ) );
        newEvent->Action            = toString( deser.GetAttribute( Serial::WIDGET_ACTION_WSTRING ) );

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

// *************************************
//
std::wstring WidgetEvent::CommandToWString    ( Command cmd )
{
    if( cmd == Command::Crawl )
        return Serial::COMMAND_CRAWL_WSTRING;
    else if( cmd == Command::Counter )
        return Serial::COMMAND_COUNTER_WSTRING;
    else
        return Serial::EMPTY_WSTRING;     // No way to be here. warning: not all control paths return value
}

// *************************************
//
WidgetEvent::Command WidgetEvent::WStringToCommand    ( const std::wstring& string )
{
    if( string == Serial::COMMAND_CRAWL_WSTRING )
        return Command::Crawl;
    else if( string == Serial::COMMAND_CRAWL_WSTRING )
        return Command::Counter;
    else
        return Command::Fail;
}

//******************* VideoCardEvent *************

// *************************************
//
void                VideoCardEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( VideoCommand ) );
    ser.SetAttribute( Serial::WIDGET_ACTION_WSTRING, toWString( Action ) );
}

// *************************************
//
IEventPtr                VideoCardEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        VideoCardEventPtr newEvent      = std::make_shared<VideoCardEvent>();
        newEvent->VideoCommand          = WStringToCommand( deser.GetAttribute( Serial::COMMAND_WSTRING ) );
        newEvent->Action                = toString( deser.GetAttribute( Serial::VIDEO_CARD_ACTION_WSTRING ) );

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

// *************************************
//
std::wstring VideoCardEvent::CommandToWString    ( Command cmd )
{
    if( cmd == Command::VideoCardOff )
        return Serial::COMMAND_VIDEO_CARD_OFF_WSTRING;
    else if( cmd == Command::VideoCardOn )
        return Serial::COMMAND_VIDEO_CARD_ON_WSTRING;
    else if( cmd == Command::KeyOn )
        return Serial::COMMAND_VIDEO_CARD_KEY_ON_WSTRING;
    else if( cmd == Command::KeyOff )
        return Serial::COMMAND_VIDEO_CARD_KEY_OFF_WSTRING;
    else
        return Serial::EMPTY_WSTRING;     // No way to be here. warning: not all control paths return value
}

// *************************************
//
VideoCardEvent::Command VideoCardEvent::WStringToCommand    ( const std::wstring& string )
{
    if( string == Serial::COMMAND_VIDEO_CARD_OFF_WSTRING )
        return Command::VideoCardOff;
    else if( string == Serial::COMMAND_VIDEO_CARD_ON_WSTRING )
        return Command::VideoCardOn;
    else if( string == Serial::COMMAND_VIDEO_CARD_KEY_ON_WSTRING )
        return Command::KeyOn;
    else if( string == Serial::COMMAND_VIDEO_CARD_KEY_OFF_WSTRING )
        return Command::KeyOff;
    else
        return Command::Fail;
}


//******************* HightmapEvent *************

// *************************************
//
void                HightmapEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( HightmapCommand ) );
    ser.SetAttribute( Serial::TIMER_HOURS_WSTRING, toWString( Hours ) );
    ser.SetAttribute( Serial::TIMER_MINUTES_WSTRING, toWString( Minutes ) );
    ser.SetAttribute( Serial::TIMER_SECONDS_WSTRING, toWString( Seconds ) );
    ser.SetAttribute( Serial::TIMER_MILLISECONDS_WSTRING, toWString( Milliseconds ) );
}

// *************************************
//
IEventPtr                HightmapEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        HightmapEventPtr newEvent   = std::make_shared<HightmapEvent>();
        newEvent->HightmapCommand   = WStringToCommand( deser.GetAttribute( Serial::COMMAND_WSTRING ) );
        newEvent->Hours             = stof( deser.GetAttribute( Serial::TIMER_HOURS_WSTRING ) );
        newEvent->Minutes           = stof( deser.GetAttribute( Serial::TIMER_MINUTES_WSTRING ) );
        newEvent->Seconds           = stof( deser.GetAttribute( Serial::TIMER_SECONDS_WSTRING ) );
        newEvent->Milliseconds      = stof( deser.GetAttribute( Serial::TIMER_MILLISECONDS_WSTRING ) );

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

// *************************************
//
std::wstring HightmapEvent::CommandToWString    ( Command cmd )
{
    if( cmd == Command::Anim )
        return Serial::COMMAND_HM_ANIM_WSTRING;
    else if( cmd == Command::Anim2 )
        return Serial::COMMAND_HM_ANIM2_WSTRING;
    else if( cmd == Command::Enable )
        return Serial::COMMAND_HM_ENABLE_WSTRING;
    else if( cmd == Command::Reset )
        return Serial::COMMAND_HM_RESET_WSTRING;
    else if( cmd == Command::Set )
        return Serial::COMMAND_HM_SET_WSTRING;
    else if( cmd == Command::Set3 )
        return Serial::COMMAND_HM_SET3_WSTRING;
    else if( cmd == Command::Show )
        return Serial::COMMAND_HM_SHOW_WSTRING;
    else if( cmd == Command::Start )
        return Serial::COMMAND_HM_START_WSTRING;
    else if( cmd == Command::Zoom )
        return Serial::COMMAND_HM_ZOOM_WSTRING;
    else if( cmd == Command::ZoomOut )
        return Serial::COMMAND_HM_ZOOM_OUT_WSTRING;
    else
        return Serial::EMPTY_WSTRING;     // No way to be here. warning: not all control paths return value
}

// *************************************
//
HightmapEvent::Command HightmapEvent::WStringToCommand    ( const std::wstring& string )
{
    if( string == Serial::COMMAND_HM_ANIM_WSTRING )
        return Command::Anim;
    else if( string == Serial::COMMAND_HM_ANIM2_WSTRING )
        return Command::Anim2;
    else if( string == Serial::COMMAND_HM_ENABLE_WSTRING )
        return Command::Enable;
    else if( string == Serial::COMMAND_HM_RESET_WSTRING )
        return Command::Reset;
    else if( string == Serial::COMMAND_HM_SET_WSTRING )
        return Command::Set;
    else if( string == Serial::COMMAND_HM_SET3_WSTRING )
        return Command::Set3;
    else if( string == Serial::COMMAND_HM_SHOW_WSTRING )
        return Command::Show;
    else if( string == Serial::COMMAND_HM_START_WSTRING )
        return Command::Start;
    else if( string == Serial::COMMAND_HM_ZOOM_WSTRING )
        return Command::Zoom;
    else if( string == Serial::COMMAND_HM_ZOOM_OUT_WSTRING )
        return Command::ZoomOut;
    else
        return Command::Fail;
}

#pragma warning( pop )

} //bv
