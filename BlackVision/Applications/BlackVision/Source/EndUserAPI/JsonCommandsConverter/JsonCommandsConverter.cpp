#include "JsonCommandsConverter.h"

#include "Serialization/JsonSpirit/JsonSpiritDeserilizeObject.h"
#include "Engine/Events/Interfaces/IEventManager.h"

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
    JsonSpiritDeserilizeObject deserializer;
    deserializer.LoadWString( eventString );

    IEventPtr newEvent = CreateEvent( deserializer );

    GetDefaultEventManager().ConcurrentQueueEvent( newEvent );
}

// ***********************
//
std::wstring        JsonCommandsConverter::PollEvent           ()
{
    return L"";
}


} //bv
