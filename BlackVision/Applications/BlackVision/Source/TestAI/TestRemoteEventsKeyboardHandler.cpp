#include "TestRemoteEventsKeyboardHandler.h"


#include "Serialization/JsonSpirit/JsonSpiritSerializeObject.h"
#include "Engine/Events/Events.h"
#include "../EndUserAPI/RemoteCommandsConverter.h"
//#include 


namespace bv
{

std::wstring LoadUtf8FileToString( const std::wstring & _filename );


// *********************************
//
TestRemoteEventsKeyboardHandler::TestRemoteEventsKeyboardHandler()
{}
TestRemoteEventsKeyboardHandler::~TestRemoteEventsKeyboardHandler()
{}

// *********************************
//
void TestRemoteEventsKeyboardHandler::HandleKey           ( unsigned char c, BVAppLogic* /*logic*/ )
{
    if( c == 't' )
    {// Test remote texture command
        std::ifstream file( "serialization/textureSerialize.json" );
        std::string assetDesc( (std::istreambuf_iterator<char>( file )), std::istreambuf_iterator<char>() );
	    file.close();

        LoadAssetEventPtr newEvent = std::make_shared<LoadAssetEvent>();
        newEvent->PluginName = "texture";
        newEvent->NodeName = "DEFAULT_CONE";
        newEvent->AssetData = std::move( assetDesc );

        JsonSpiritSerializeObject ser;
        newEvent->Serialize( ser );

        std::wstring pretendRemoteString = ser.Save( FORMATSTYLE_SPARING );

        RemoteCommandsConverter::GetRemoteCommandsConverter().QueueEvent( pretendRemoteString );
    }
    else if( c == 'r' )
    {// Test remote commands magic
        std::wstring pretendRemoteString = LoadUtf8FileToString( L"serialization/remoteEvent.json" );

        RemoteCommandsConverter::GetRemoteCommandsConverter().QueueEvent( pretendRemoteString );
    }
    else if( c == 'e' )
    {// Save descriptor. This helps to create remote event. You can copy this desc to serialization/remoteEvent.json file.
        ParamKeyEventPtr newEvent = std::make_shared<ParamKeyEvent>();
        newEvent->PluginName = "texture";
        newEvent->NodeName = "DEFAULT_CONE";
        newEvent->KeyCommand = ParamKeyEvent::Command::AddKey;
        newEvent->ParamName = "translate";
        newEvent->Value = L"3.14";
        newEvent->Time = 3.0f;

        JsonSpiritSerializeObject ser;
        newEvent->Serialize( ser );

        ser.Save( "serialization/eventSerialize.json", FORMATSTYLE_READABLE );
    }
}


} //bv
