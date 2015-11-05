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

// Pawełek event pevent

const EventType VideoCardEvent::m_sEventType       = 0x10000005;
std::string VideoCardEvent::m_sEventName           = "Event_VideoCard";

const EventType SetParamEvent::m_sEventType       = 0x10000006;
std::string SetParamEvent::m_sEventName           = "Event_SetParam";

const EventType TimeLineCmd::m_sEventType       = 0x10000009;
std::string TimeLineCmd::m_sEventName           = "Event_Timeline";

const EventType TimerCmd::m_sEventType       = 0x10000010;
std::string TimerCmd::m_sEventName           = "Event_Timer";

const EventType WidgetCmd::m_sEventType       = 0x10000011;
std::string WidgetCmd::m_sEventName           = "Event_Widget";

const EventType SceneStructureEventDeprecated::m_sEventType       = 0x10000012;
std::string SceneStructureEventDeprecated::m_sEventName           = "Event_SceneStructure";

const EventType InfoEvent::m_sEventType             = 0x10000007;
std::string InfoEvent::m_sEventName                 = "Event_Info";



// ************************************* new Events *****************************************
const EventType LoadAssetEvent::m_sEventType        = 0x10000013;
std::string LoadAssetEvent::m_sEventName            = "Event_LoadAsset";

const EventType ParamKeyEvent::m_sEventType         = 0x30000006;
std::string ParamKeyEvent::m_sEventName             = "Event_ParamKeyEvent";

const EventType SceneStructureEvent::m_sEventType   = 0x30000012;
std::string SceneStructureEvent::m_sEventName       = "Event_SceneStructure";

const EventType ProjectEvent::m_sEventType          = 0x30000013;
std::string ProjectEvent::m_sEventName              = "Event_ProjectStructure";

const EventType ResponseEvent::m_sEventType         = 0x30000008;
std::string ResponseEvent::m_sEventName             = "Event_Response";

const EventType NewInfoEvent::m_sEventType          = 0x30000007;
std::string NewInfoEvent::m_sEventName              = "Event_Info";

const EventType TimeLineEvent::m_sEventType         = 0x30000009;
std::string TimeLineEvent::m_sEventName             = "Event_Timeline";

// ************************************* Events Serialization *****************************************

namespace Serial
{
// Key names used to serialization.

const std::wstring EMPTY_WSTRING            = L"";

const std::wstring EVENT_TYPE_WSTRING       = L"cmd";
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

// ScenStructureEvent
const std::wstring NEW_NODE_NAME_WSTRING        = L"NewNodeName";

const std::wstring COMMAND_ADD_NODE_WSTRING         = L"AddNode";
const std::wstring COMMAND_REMOVE_NODE_WSTRING      = L"RemoveNode";
const std::wstring COMMAND_ATTACH_PLUGIN_WSTRING    = L"AttachPlugin";
const std::wstring COMMAND_DETACH_PLUGIN_WSTRING    = L"DetachPlugin";
const std::wstring COMMAND_SET_NODE_VISIBLE_WSTRING     = L"SetNodeVisible";
const std::wstring COMMAND_SET_NODE_INVISIBLE_WSTRING   = L"SetNodeInvisible";

// ProjectEvent
const std::wstring REQUEST_WSTRING                          = L"request";

const std::wstring COMMAND_SAVE_SCENE_WSTRING               = L"SAVE_SCENE";
const std::wstring COMMAND_LOAD_PROJECT_WSTRING             = L"LOAD_PROJECT";
const std::wstring COMMAND_NEW_PROJECT_WSTRING              = L"NEW_PROJECT";
const std::wstring COMMAND_SET_CURRENT_PROJECT_WSTRING      = L"SET_CURRENT_PROJECT";
const std::wstring COMMAND_LIST_PROJECTS_NAMES_WSTRING      = L"LIST_PROJECTS_NAMES";
const std::wstring COMMAND_LIST_SCENES_WSTRING              = L"LIST_SCENES";
const std::wstring COMMAND_LIST_ASSETS_PATHS_WSTRING        = L"LIST_ASSETS_PATHS";
const std::wstring COMMAND_LIST_CATEGORIES_NAMES_WSTRING    = L"LIST_CATEGORIES_NAMES";
const std::wstring COMMAND_LIST_PROJECTS_WSTRING            = L"LIST_PROJECTS";

// ResponseEvent
const std::wstring RESPONSE_WSTRING                     = L"response";

// InfoEvent
const std::wstring COMMAND_TREE_STRUCTURE_WSTRING       = L"tree_structure";
const std::wstring COMMAND_PERFORMANCE_WSTRING          = L"performance";
const std::wstring COMMAND_TIMELINES_WSTRING            = L"timelines";
const std::wstring COMMAND_NODE_INFO_WSTRING            = L"node_info";
const std::wstring COMMAND_VIDEO_CARDS_WSTRING          = L"videocards";

// TimelineEvent
const std::wstring COMMAND_PLAY_WSTRING                 = L"play";
const std::wstring COMMAND_STOP_WSTRING                 = L"stop";
const std::wstring COMMAND_PLAY_REVERSE_WSTRING         = L"play_reverse";
const std::wstring COMMAND_GOTO_WSTRING                 = L"goto";
const std::wstring COMMAND_GOTO_AND_PLAY_WSTRING        = L"gotoandplay";

const std::wstring TIMELINE_TIME_VALUE_WSTRING          = L"time";
const std::wstring TIMELINE_NAME_WSTRING                = L"time";
}

// ************************************* PluginAddedEvent *************************************

// *************************************
//
PluginAddedEvent::PluginAddedEvent                          ()
    : m_addedPlugin( nullptr )
{
}

// *************************************
//
PluginAddedEvent::PluginAddedEvent                          ( model::IPluginConstPtr plugin )
    : m_addedPlugin( plugin )
{
}

// *************************************
//
EventType               PluginAddedEvent::GetEventType      () const
{
    return m_sEventType;
}

// *************************************
//
void                    PluginAddedEvent::Serialize         ( ISerializer& /*ser*/ ) const
{
    std::string err = GetName() + " should not be serialized";
    assert( !err.c_str()  );
}

// *************************************
//
IEventPtr                    PluginAddedEvent::Create          ( IDeserializer& /*deser*/ )
{
    std::string err = m_sEventName + " should not be deserialized";
    assert( !err.c_str()  );
    return nullptr;
}

// *************************************
//
IEventPtr               PluginAddedEvent::Clone             () const
{
    return IEventPtr( new PluginAddedEvent( *this ) );
}

// *************************************
//
const std::string &     PluginAddedEvent::GetName           () const
{
    return m_sEventName;
}

// *************************************
//
model::IPluginConstPtr  PluginAddedEvent::GetPlugin         () const
{
    return m_addedPlugin;
}

// *************************************
//
EventType               PluginAddedEvent::Type              ()
{
    return m_sEventType;
}


// ************************************* NodeAddedEvent *************************************

// *************************************
//
NodeAddedEvent::NodeAddedEvent                              ()
    : m_addedNode( nullptr )
{
}

// *************************************
//
NodeAddedEvent::NodeAddedEvent                              ( model::IModelNodeConstPtr node )
    : m_addedNode( node )
{
}

// *************************************
//
EventType               NodeAddedEvent::GetEventType        () const
{
    return m_sEventType;
}

// *************************************
//
void                    NodeAddedEvent::Serialize           ( ISerializer& ser ) const
{
    { ser; } // FIXME: suppress unused warning
    std::string err = GetName() + " should not be serialized";
    assert( !err.c_str()  );
}

// *************************************
//
IEventPtr               NodeAddedEvent::Create          ( IDeserializer& /*deser*/ )
{
    std::string err = m_sEventName + " should not be deserialized";
    assert( !err.c_str()  );
    return nullptr;
}

// *************************************
//
IEventPtr               NodeAddedEvent::Clone               () const
{
    return IEventPtr( new NodeAddedEvent( *this ) );
}

// *************************************
//
const std::string &     NodeAddedEvent::GetName             () const
{
    return m_sEventName;
}

// *************************************
//
model::IModelNodeConstPtr  NodeAddedEvent::GetNode         () const
{
    return m_addedNode;
}

// *************************************
//
EventType                   NodeAddedEvent::Type            ()
{
    return m_sEventType;
}


// ************************************* FrameRenderedEvent *************************************

// *************************************
//
FrameRenderedEvent::FrameRenderedEvent                          ( bv::Renderer * renderer )
    : m_renderer( renderer )
{
}

// *************************************
//
EventType               FrameRenderedEvent::GetEventType        () const
{
    return m_sEventType;
}

// *************************************
//
void                    FrameRenderedEvent::Serialize           ( ISerializer& ser ) const
{
    { ser; } // FIXME: suppress unused warning
    std::string err = GetName() + " should not be serialized";
    assert( !err.c_str()  );
}

// *************************************
//
IEventPtr                    FrameRenderedEvent::Create          ( IDeserializer& deser )
{
    { deser; } // FIXME: suppress unused warning
    std::string err = m_sEventName + " should not be deserialized";
    assert( !err.c_str()  );
    return nullptr;
}

// *************************************
//
IEventPtr               FrameRenderedEvent::Clone               () const
{
    return IEventPtr( new FrameRenderedEvent( *this ) );
}

// *************************************
//
const std::string &     FrameRenderedEvent::GetName             () const
{
    return m_sEventName;
}

// *************************************
//
Renderer *              FrameRenderedEvent::Renderer            ()
{
    return m_renderer;
}

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
{
    return m_width;
}

// *************************************
//
int                     FrameRenderedEvent::Height              () const
{
    return m_height;
}

// *************************************
//
EventType               FrameRenderedEvent::Type                ()
{
    return m_sEventType;
}


// ************************************* FrameRenderedEvent *************************************

// *************************************
//
TransformSetEvent::TransformSetEvent                        ()
    : m_translation( 0.f, 0.f, 0.f )
    , m_scale( 1.f, 1.f, 1.f )
{
}

// *************************************
//
EventType           TransformSetEvent::GetEventType         () const
{
    return m_sEventType;
}

// *************************************
//
void                TransformSetEvent::Serialize            ( ISerializer& ser ) const
{
    { ser; } // FIXME: suppress unused warning
    assert( false );
}

// *************************************
//
IEventPtr                TransformSetEvent::Create          ( IDeserializer& deser )
{
    { deser; } // FIXME: suppress unused warning
    assert( false );
    return nullptr;
}

// *************************************
//
IEventPtr           TransformSetEvent::Clone                () const
{
    return IEventPtr( new TransformSetEvent( *this ) );
}

// *************************************
//
const std::string & TransformSetEvent::GetName              () const
{
    return m_sEventName;
}

// *************************************
//
void                TransformSetEvent::SetTranslation       ( const glm::vec3 & translation )
{
    m_translation = translation;
}

// *************************************
//
void                TransformSetEvent::SetScale             ( const glm::vec3 & scale )
{
    m_scale = scale;
}

// *************************************
//
const glm::vec3 &   TransformSetEvent::Translation          () const
{
    return m_translation;
}

// *************************************
//
const glm::vec3 &   TransformSetEvent::Scale                () const
{
    return m_scale;
}

// *************************************
//
EventType           TransformSetEvent::Type                 ()
{
    return m_sEventType;
}

// *************************************
//
KeyPressedEvent::KeyPressedEvent        ()
{}

// *************************************
//
EventType               KeyPressedEvent::GetEventType      () const
{
    return m_sEventType;
}

// *************************************
//
IEventPtr               KeyPressedEvent::Clone             () const
{
    return IEventPtr( new KeyPressedEvent( *this ) );
}

// *************************************
//
const std::string &     KeyPressedEvent::GetName           () const
{
    return m_sEventName;
}

// *************************************
//
void                    KeyPressedEvent::SetChar             ( unsigned char  c )
{
    m_char = c;
}

// *************************************
//
unsigned char           KeyPressedEvent::GetChar             () const
{
    return m_char;
}

// *************************************
//
EventType               KeyPressedEvent::Type                ()
{
    return m_sEventType;
}


// ------------ Pawe�ek event 

VideoCardEvent::VideoCardEvent         () 
{
  
}


// *************************************
//
EventType           VideoCardEvent::GetEventType         () const
{
    return this->m_sEventType;
}

// *************************************
//
void                VideoCardEvent::Serialize            ( ISerializer& ser ) const
{
    assert( false );
}

// *************************************
//
IEventPtr                VideoCardEvent::Create          ( IDeserializer& deser )
{
    assert( false );
    return nullptr;
}
// *************************************
//
IEventPtr               VideoCardEvent::Clone             () const
{
    return IEventPtr( new VideoCardEvent( *this ) );
}
// *************************************
//
EventType               VideoCardEvent::Type              ()
{
    return m_sEventType;
}

// *************************************
//
const std::string &     VideoCardEvent::GetName           () const
{
    return m_sEventName;
}




InfoEvent::InfoEvent         () 
{
  
}


// *************************************
//
EventType           InfoEvent::GetEventType         () const
{
    return this->m_sEventType;
}

// *************************************
//
void                InfoEvent::Serialize            ( ISerializer& ser ) const
{
    assert( false );
}

// *************************************
//
IEventPtr                InfoEvent::Create          ( IDeserializer& deser )
{
    assert( false );
    return nullptr;
}
// *************************************
//
IEventPtr               InfoEvent::Clone             () const
{
    return IEventPtr( new InfoEvent( *this ) );
}
// *************************************
//
EventType               InfoEvent::Type              ()
{
    return m_sEventType;
}

// *************************************
//
bool                    InfoEvent::ForceSync        () const
{
    return request == L"grab_that_frame";
}

// *************************************
//
const std::string &     InfoEvent::GetName           () const
{
    return m_sEventName;
}

// *************************************
//
const std::wstring &         InfoEvent::GetAddStrData    () const
{
    return m_additionalStrData;
}





SceneStructureEventDeprecated::SceneStructureEventDeprecated         () 
{}

// *************************************
//
EventType           SceneStructureEventDeprecated::GetEventType         () const
{
    return this->m_sEventType;
}

// *************************************
//
void                SceneStructureEventDeprecated::Serialize            ( ISerializer& ser ) const
{
    assert( false );
}

// *************************************
//
IEventPtr                SceneStructureEventDeprecated::Create          ( IDeserializer& deser )
{
    assert( false );
    return nullptr;
}
// *************************************
//
IEventPtr               SceneStructureEventDeprecated::Clone             () const
{
    return IEventPtr( new SceneStructureEventDeprecated( *this ) );
}
// *************************************
//
EventType               SceneStructureEventDeprecated::Type              ()
{
    return m_sEventType;
}

// *************************************
//
bool                    SceneStructureEventDeprecated::ForceSync        () const
{
    return request == L"grab_that_frame";
}

// *************************************
//
const std::string &     SceneStructureEventDeprecated::GetName           () const
{
    return m_sEventName;
}

// *************************************
//
const std::wstring &         SceneStructureEventDeprecated::GetAddStrData    () const
{
    return m_additionalStrData;
}


//******************* SET PARAM *************



SetParamEvent::SetParamEvent         () 
{
  
}


// *************************************
//
EventType           SetParamEvent::GetEventType         () const
{
    return this->m_sEventType;
}

// *************************************
//
void                SetParamEvent::Serialize            ( ISerializer& ser ) const
{
    assert( false );
}

// *************************************
//
IEventPtr                SetParamEvent::Create          ( IDeserializer& deser )
{
    assert( false );
    return nullptr;
}
// *************************************
//
IEventPtr               SetParamEvent::Clone             () const
{
    return IEventPtr( new SetParamEvent( *this ) );
}
// *************************************
//
EventType               SetParamEvent::Type              ()
{
    return m_sEventType;
}

// *************************************
//
const std::string &     SetParamEvent::GetName           () const
{
    return m_sEventName;
}




//******************* Timeline CMD *************



TimeLineCmd::TimeLineCmd         () 
{
  
}


// *************************************
//
EventType           TimeLineCmd::GetEventType         () const
{
    return this->m_sEventType;
}

// *************************************
//
void                TimeLineCmd::Serialize            ( ISerializer& ser ) const
{
    assert( false );
}

// *************************************
//
IEventPtr                TimeLineCmd::Create          ( IDeserializer& deser )
{
    assert( false );
    return nullptr;
}
// *************************************
//
IEventPtr               TimeLineCmd::Clone             () const
{
    return IEventPtr( new TimeLineCmd( *this ) );
}
// *************************************
//
EventType               TimeLineCmd::Type              ()
{
    return m_sEventType;
}

// *************************************
//
const std::string &     TimeLineCmd::GetName           () const
{
    return m_sEventName;
}








//******************* Timer CMD *************



TimerCmd::TimerCmd         () 
{
  
}


// *************************************
//
EventType           TimerCmd::GetEventType         () const
{
    return this->m_sEventType;
}

// *************************************
//
void                TimerCmd::Serialize            ( ISerializer& ser ) const
{
    assert( false );
}

// *************************************
//
IEventPtr                TimerCmd::Create          ( IDeserializer& deser )
{
    assert( false );
    return nullptr;
}
// *************************************
//
IEventPtr               TimerCmd::Clone             () const
{
    return IEventPtr( new TimerCmd( *this ) );
}
// *************************************
//
EventType               TimerCmd::Type              ()
{
    return m_sEventType;
}

// *************************************
//
const std::string &     TimerCmd::GetName           () const
{
    return m_sEventName;
}







//******************* Widget CMD *************



WidgetCmd::WidgetCmd         () 
{
  
}


// *************************************
//
EventType           WidgetCmd::GetEventType         () const
{
    return this->m_sEventType;
}

// *************************************
//
void                WidgetCmd::Serialize            ( ISerializer& ser ) const
{
    assert( false );
}

// *************************************
//
IEventPtr                WidgetCmd::Create          ( IDeserializer& deser )
{
    assert( false );
    return nullptr;
}
// *************************************
//
IEventPtr               WidgetCmd::Clone             () const
{
    return IEventPtr( new WidgetCmd( *this ) );
}
// *************************************
//
EventType               WidgetCmd::Type              ()
{
    return m_sEventType;
}

// *************************************
//
const std::string &     WidgetCmd::GetName           () const
{
    return m_sEventName;
}




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





//******************* SceneStructureEvent *************

// *************************************
//
void                SceneStructureEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::NODE_NAME_WSTRING, toWString( NodeName ) );
    ser.SetAttribute( Serial::NEW_NODE_NAME_WSTRING, toWString( NewNodeName ) );
    ser.SetAttribute( Serial::PLUGIN_NAME_WSTRING, toWString( PluginName ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( SceneCommand ) );
}

// *************************************
//
IEventPtr                SceneStructureEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        SceneStructureEventPtr newEvent   = std::make_shared<SceneStructureEvent>();
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
IEventPtr               SceneStructureEvent::Clone             () const
{   return IEventPtr( new SceneStructureEvent( *this ) );  }

// *************************************
//
EventType           SceneStructureEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        SceneStructureEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  SceneStructureEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           SceneStructureEvent::GetEventType() const
{   return this->m_sEventType; }

// *************************************
//
std::wstring SceneStructureEvent::CommandToWString    ( Command cmd )
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
SceneStructureEvent::Command SceneStructureEvent::WStringToCommand    ( const std::wstring& string )
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
void                NewInfoEvent::Serialize            ( ISerializer& ser ) const
{
    ser.SetAttribute( Serial::EVENT_TYPE_WSTRING, toWString( m_sEventName ) );
    ser.SetAttribute( Serial::COMMAND_WSTRING, CommandToWString( InfoRequest ) );
    ser.SetAttribute( Serial::NODE_NAME_WSTRING, toWString( NodeName ) );
}

// *************************************
//
IEventPtr                NewInfoEvent::Create          ( IDeserializer& deser )
{
    if( deser.GetAttribute( Serial::EVENT_TYPE_WSTRING ) == toWString( m_sEventName ) )
    {
        NewInfoEventPtr newEvent    = std::make_shared<NewInfoEvent>();
        newEvent->NodeName          = toString( deser.GetAttribute( Serial::NODE_NAME_WSTRING ) );
        newEvent->InfoRequest       = WStringToCommand( deser.GetAttribute( Serial::COMMAND_WSTRING ) );
        
        return newEvent;
    }
    return nullptr;    
}
// *************************************
//
IEventPtr               NewInfoEvent::Clone             () const
{   return IEventPtr( new NewInfoEvent( *this ) );  }

// *************************************
//
EventType           NewInfoEvent::Type()
{   return m_sEventType;   }
// *************************************
//
std::string&        NewInfoEvent::Name()
{   return m_sEventName;   }
// *************************************
//
const std::string&  NewInfoEvent::GetName() const
{   return Name();   }
// *************************************
//
EventType           NewInfoEvent::GetEventType() const
{   return this->m_sEventType; }

// *************************************
//
std::wstring NewInfoEvent::CommandToWString    ( Command cmd )
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
NewInfoEvent::Command NewInfoEvent::WStringToCommand    ( const std::wstring& string )
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

#pragma warning( pop )

} //bv
