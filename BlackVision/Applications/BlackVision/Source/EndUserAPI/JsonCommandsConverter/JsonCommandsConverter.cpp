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
void                JsonCommandsConverter::QueueEvent          ( const std::wstring& eventString )
{
    JsonSpiritDeserializeObject deserializer;

    if( !deserializer.LoadWString( eventString ) )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Commands converter can't parse command: \n" + toString( eventString );
        return;
    }

    IEventPtr newEvent = CreateEvent( deserializer );

    GetDefaultEventManager().ConcurrentQueueEvent( newEvent );
}

// ***********************
// Always check, if string isn't empty. That means, no more events are queued.
std::wstring        JsonCommandsConverter::PollEvent           ()
{
    auto evt = GetDefaultEventManager().GetNextResponse();
    if( evt == nullptr )
        return L"";

    // Maybe later, but for now it must be compatibile with previous solution.
    //JsonSpiritSerializeObject ser;
    //evt->Serialize( ser );

    if( evt->GetEventType() != ResponseEvent::Type() )
        return L"";

    ResponseEventPtr response = std::static_pointer_cast<ResponseEvent>( evt );
    return std::move( response->Response );
}


} //bv
