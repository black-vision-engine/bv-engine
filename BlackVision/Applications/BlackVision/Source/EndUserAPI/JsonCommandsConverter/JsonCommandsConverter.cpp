#include "JsonCommandsConverter.h"

#include "Serialization/JsonSpirit/JsonSpiritDeserilizeObject.h"
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
    JsonSpiritDeserilizeObject deserializer;

    if( !deserializer.LoadWString( eventString ) )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Commands converter can't parse command: \n" + toString( eventString );
        return;
    }

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
