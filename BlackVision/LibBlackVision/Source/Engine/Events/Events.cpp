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

const EventType InfoEvent::m_sEventType       = 0x10000007;
std::string InfoEvent::m_sEventName           = "Event_Info";

const EventType ResponseEvent::m_sEventType       = 0x10000008;
std::string ResponseEvent::m_sEventName           = "Event_Response";

const EventType TimeLineCmd::m_sEventType       = 0x10000009;
std::string TimeLineCmd::m_sEventName           = "Event_Timeline";

const EventType TimerCmd::m_sEventType       = 0x10000010;
std::string TimerCmd::m_sEventName           = "Event_Timer";

const EventType WidgetCmd::m_sEventType       = 0x10000011;
std::string WidgetCmd::m_sEventName           = "Event_Widget";

const EventType SceneStructureEvent::m_sEventType       = 0x10000012;
std::string SceneStructureEvent::m_sEventName           = "Event_SceneStructure";

const EventType LoadAssetEvent::m_sEventType            = 0x10000013;
std::string LoadAssetEvent::m_sEventName                = "Event_LoadAsset";


// ************************************* new Events *****************************************
const EventType ParamKeyEvent::m_sEventType       = 0x30000006;
std::string ParamKeyEvent::m_sEventName           = "Event_ParamKeyEvent";



// ************************************* Events Serialization *****************************************

namespace Serial
{
const std::wstring EMPTY_WSTRING            = L"";

const std::wstring EVENT_TYPE_WSTRING          = L"cmd";
const std::wstring NODE_NAME_WSTRING        = L"NodeName";
const std::wstring PLUGIN_NAME_WSTRING      = L"PluginName";

// LoadAssetEvent
const std::wstring ASSET_DATA_WSTRING       = L"AssetData";

// ParamKeyEvent
const std::wstring PARAM_NAME_WSTRING       = L"ParamName";
const std::wstring PARAM_VALUE_WSTRING      = L"ParamValue";
const std::wstring KEY_TIME_WSTRING         = L"Time";
const std::wstring KEY_OPERATION_WSTRING    = L"Command";
const std::wstring ADD_KEY_COMMAND_WSTRING  = L"AddKey";
const std::wstring REMOVE_KEY_COMMAND_WSTRING   = L"RemoveKey";
const std::wstring UPDATE_KEY_COMMAND_WSTRING   = L"UpdateKey";

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





SceneStructureEvent::SceneStructureEvent         () 
{
  
}


// *************************************
//
EventType           SceneStructureEvent::GetEventType         () const
{
    return this->m_sEventType;
}

// *************************************
//
void                SceneStructureEvent::Serialize            ( ISerializer& ser ) const
{
    assert( false );
}

// *************************************
//
IEventPtr                SceneStructureEvent::Create          ( IDeserializer& deser )
{
    assert( false );
    return nullptr;
}
// *************************************
//
IEventPtr               SceneStructureEvent::Clone             () const
{
    return IEventPtr( new SceneStructureEvent( *this ) );
}
// *************************************
//
EventType               SceneStructureEvent::Type              ()
{
    return m_sEventType;
}

// *************************************
//
bool                    SceneStructureEvent::ForceSync        () const
{
    return request == L"grab_that_frame";
}

// *************************************
//
const std::string &     SceneStructureEvent::GetName           () const
{
    return m_sEventName;
}

// *************************************
//
const std::wstring &         SceneStructureEvent::GetAddStrData    () const
{
    return m_additionalStrData;
}






ResponseEvent::ResponseEvent         () 
{
  
}


// *************************************
//
EventType           ResponseEvent::GetEventType         () const
{
    return this->m_sEventType;
}

// *************************************
//
void                ResponseEvent::Serialize            ( ISerializer& ser ) const
{
    assert( false );
}

// *************************************
//
IEventPtr                ResponseEvent::Create          ( IDeserializer& deser )
{
    assert( false );
    return nullptr;
}
// *************************************
//
IEventPtr               ResponseEvent::Clone             () const
{
    return IEventPtr( new ResponseEvent( *this ) );
}
// *************************************
//
EventType               ResponseEvent::Type              ()
{
    return m_sEventType;
}

// *************************************
//
const std::string &     ResponseEvent::GetName           () const
{
    return m_sEventName;
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
    ser.SetAttribute( Serial::KEY_OPERATION_WSTRING, CommandToWString( KeyCommand ) );
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
        newEvent->KeyCommand        = WStringToCommand( deser.GetAttribute( Serial::KEY_OPERATION_WSTRING ) );
        
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
        return Serial::ADD_KEY_COMMAND_WSTRING;
    else if( cmd == Command::RemoveKey )
        return Serial::REMOVE_KEY_COMMAND_WSTRING;
    else if( cmd == Command::UpdateKey )
        return Serial::UPDATE_KEY_COMMAND_WSTRING;
    else
        return Serial::EMPTY_WSTRING;     // No way to be here. warning: not all control paths return value
}
// *************************************
//
ParamKeyEvent::Command ParamKeyEvent::WStringToCommand    ( const std::wstring& string )
{
    if( string == Serial::ADD_KEY_COMMAND_WSTRING )
        return Command::AddKey;
    else if( string == Serial::REMOVE_KEY_COMMAND_WSTRING )
        return Command::RemoveKey;
    else if( string == Serial::UPDATE_KEY_COMMAND_WSTRING )
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

#pragma warning( pop )

} //bv
