#include "TestRemoteEventsKeyboardHandler.h"


#include "Serialization/JsonSpirit/JsonSpiritSerializeObject.h"
#include "Engine/Events/Events.h"
#include "../EndUserAPI/RemoteCommandsConverter.h"


namespace bv
{

std::wstring LoadUtf8FileToString( const std::wstring & _filename );        // There's no header. Implementation in file FreeTypeEngine.cpp LibBlackVision.


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
    {// Save descriptor. This helps to create remote event. You can copy this desc to serialization/remoteEvent.json.
        SceneStructureEventPtr newEvent = std::make_shared<SceneStructureEvent>();
        newEvent->NodeName = "root";
        newEvent->SceneCommand = SceneStructureEvent::Command::RemoveNode;
        newEvent->NewNodeName = "";
        newEvent->PluginName = "";

        JsonSpiritSerializeObject ser;
        newEvent->Serialize( ser );

        ser.Save( "serialization/eventSerialize.json", FORMATSTYLE_READABLE );
    }
    else if( c == 'i' )
    {// Append info response to file.
        std::wofstream file( L"serialization/responseEvents.json", std::ios_base::app );
        
        auto infoResponse = RemoteCommandsConverter::GetRemoteCommandsConverter().PollEvent();
        while( infoResponse != L"" )
        {            
            file << infoResponse << std::endl;
            infoResponse = RemoteCommandsConverter::GetRemoteCommandsConverter().PollEvent();
        }

        file.close();
    }
}


} //bv
