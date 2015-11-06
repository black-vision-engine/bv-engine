#include "JsonCommandsConverter.h"

#include "Serialization/JsonSpirit/JsonSpiritDeserializeObject.h"
#include "Serialization/JsonSpirit/JsonSpiritSerializeObject.h"
#include "Engine/Events/Interfaces/IEventManager.h"
#include "Engine/Events/EventHelpers.h"

#include "UseLogger.h"

namespace bv
{

// ***********************
//
JsonCommandsConverter::JsonCommandsConverter()
{}

JsonCommandsConverter::~JsonCommandsConverter()
{}

// ***********************
//
/// @param[in] eventString String to parse.
/// @param[in] socketID Event sender identifier.
void                JsonCommandsConverter::QueueEvent          ( const std::wstring& eventString, int socketID )
{
    JsonSpiritDeserializeObject deserializer;

    if( !deserializer.LoadWString( eventString ) )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Commands converter can't parse command: \n" + toString( eventString );
        return;
    }

    BaseEventPtr newEvent = std::static_pointer_cast<BaseEvent>( CreateEvent( deserializer ) );
    newEvent->SocketID = socketID;

    if( newEvent != nullptr )
        GetDefaultEventManager().ConcurrentQueueEvent( newEvent );
}

// ***********************
// Always check, if string isn't empty. That means, no more events are queued.
ResponseEventPtr        JsonCommandsConverter::PollEvent           ()
{
    auto evt = GetDefaultEventManager().GetNextResponse();
    if( evt == nullptr )
        return nullptr;

    if( evt->GetEventType() != ResponseEvent::Type() )
        return nullptr;

    return std::static_pointer_cast< ResponseEvent >( evt );
}


} //bv
