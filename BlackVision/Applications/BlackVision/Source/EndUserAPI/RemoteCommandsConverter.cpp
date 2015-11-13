#include "RemoteCommandsConverter.h"

#include "Serialization/IDeserializer.h"
#include "Engine/Events/Events.h"
#include "JsonCommandsConverter/JsonCommandsConverter.h"

namespace bv
{

// ***********************
//
RemoteCommandsConverter::RemoteCommandsConverter()
{
    RegisterEvent( LoadAssetEvent::Name(), LoadAssetEvent::Create );
    RegisterEvent( ParamKeyEvent::Name(), ParamKeyEvent::Create );
    RegisterEvent( NodeStructureEvent::Name(), NodeStructureEvent::Create );
    RegisterEvent( ProjectEvent::Name(), ProjectEvent::Create );
    RegisterEvent( InfoEvent::Name(), InfoEvent::Create );
    RegisterEvent( TimeLineEvent::Name(), TimeLineEvent::Create );
    RegisterEvent( TimerEvent::Name(), TimerEvent::Create );
}

// ***********************
// @todo Move creation of commands converter to better place. Maybe BVAppLogic ??
RemoteCommandsConverter& RemoteCommandsConverter::GetRemoteCommandsConverter()
{
    static JsonCommandsConverter converter;
    return static_cast<RemoteCommandsConverter&>( converter );
}


// ***********************
//
IEventPtr RemoteCommandsConverter::CreateEvent         ( IDeserializer& deser )
{
    std::wstring command = deser.GetAttribute( L"Event" );
    auto iter = m_eventsConverter.find( command );
    if( iter != m_eventsConverter.end() )
    {
        auto eventCreator = iter->second;
        return eventCreator( deser );
    }
    else
        return nullptr;
}

// ***********************
// Command name and event name are to separate things. Command names
void RemoteCommandsConverter::RegisterEvent       ( const std::string& eventName, EventCreatorDelegate eventCreator )
{
    std::wstring commandName = std::wstring( eventName.begin(), eventName.end() );
    m_eventsConverter[ commandName ] = eventCreator;
}


}//bv
